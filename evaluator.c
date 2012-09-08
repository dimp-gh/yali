#include "evaluator.h"
#include "common.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>


SExpression *eval(SExpression *expr, SymbolTable *ST) {
  /*
   * if SExpression is pair AND
   *    head of SExpression is lambda or function name:
   *      handle it as function call: eval all following terms and apply function to them.
   * else if SExpression is special form call:
   *     handle it as special form call
   *	  special forms:
   *	    define
   *	    cons, car, cdr
   *	    cond, if
   *	    or, and,
   *	  type predicates:
   *	    null?
   *	    int?
   *	    pair?
   * else if SExpression is one elementary-typed element:
   *     return it
   * otheriwse:
   *     error, can't eval this crap	
   */
  if (!expr)
    return NULL;
  printf("Evaluating "); print_typed_expression(expr);
  if (expr->type == tt_mention)
    return ht_lookup(ST, expr->mention);
  else if (expr->type == tt_int ||
	   expr->type == tt_bool)
    return expr;
  else if (expr->type == tt_lambda)
    return expr;
  else if (expr->type != tt_pair) // if expression is one elementary-typed element
    return expr;
  else if (!expr->pair->value) // if expression is empty list
    return NULL;
  else { // if expression is pair and isn't empty
    SExpression *head = expr->pair->value, *tail = expr->pair->next;
    if (head->type != tt_mention) // if head of list is not function call
      return NULL;
    char *call = head->mention;
    // checking for special forms
    if (strcmp(call, "define") == 0)
      return handle_define(tail, ST);
    else if (strcmp(call, "if") == 0)
      return handle_if(tail, ST);
    else if (strcmp(call, "=") == 0)
      return handle_eq(tail, ST);
    else if (strcmp(call, "--") == 0)
      return handle_dec(tail, ST);
    else if (strcmp(call, "*") == 0)
      return handle_mul(tail, ST);
    else if (strcmp(call, "+") == 0)
      return handle_plus(tail, ST);
    else if (strcmp(call, "-") == 0)
      return handle_minus(tail, ST);
    // special forms check end
    else {
      SExpression *function = ht_lookup(ST, call);
      if (!function) // undefined name
	return NULL;
      SExpression *args = duplicate_expression(tail);
      return apply(function, args, ST);
    }
  }
}

SExpression *substitute_mention(SExpression *source, char *key, SExpression *value) {
  if (!source)
    return NULL;
  if (source->type == tt_mention &&
      strcmp(source->mention, key) == 0) {
    switch (value->type) {
    case tt_int:
      source->type = tt_int;
      source->integer = value->integer;
      break;
    case tt_bool:
      source->type = tt_bool;
      source->boolean = value->boolean;
      break;
    case tt_nil:
      source->type = tt_nil;
      break;
    case tt_pair:
      source->type = tt_pair;
      source->pair->value = duplicate_expression(value->pair->value);
      source->pair->next = duplicate_expression(value->pair->next);
      break;
    case tt_lambda:
      source->type = tt_lambda;
      source->lambda->arity = value->lambda->arity;
      source->lambda->args = duplicate_expression(value->lambda->args);
      source->lambda->body = duplicate_expression(value->lambda->body);
      break;
    default:
      break;
    }
  }
  else if (source->type == tt_pair) {
    SExpression *current = source;
    while (current) {
      substitute_mention(current->pair->value, key, value);
      current = current->pair->next;
    }
  }
  return source;
}
  

SExpression *apply(SExpression *function, SExpression *args, SymbolTable *ST) {
  /*
   *  if lambda arity is 0 and there's no arguments:
   *      eval lambda body
   *  if arguments count is equal to lambda arity:
   *      apply function to argument list
   *      that is,
   *      make a substitution table (mention from lambda args)=>(SExpression from arglist) 
   *      take lambda body and substitute all mentions with args-SExpressions
   *      eval resulted expression
   */
  printf("Handling Apply.\n");
  printf("Args: "); print_expression(function->lambda->args);
  printf("Params: "); print_expression(args);
  printf("Body: "); print_expression(function->lambda->body);
  SExpression *result = NULL;
  if ((function->lambda->arity == 0) && 
      (list_length(args) == 0))
     return eval(function->lambda->body, ST);
  else if ((list_length(args) == 1) &&
	   (function->lambda->arity == 1)) {
    SExpression *subs = substitute_mention(duplicate_expression(function->lambda->body),
					   function->lambda->args->pair->value->mention,
					   eval(args->pair->value, ST));
    printf("After substitution: "); print_expression(subs);
    return eval(subs, ST);
  }
  return result;
}


SExpression *handle_define(SExpression *ex, SymbolTable *ST) {
  if (!ex || 
      list_length(ex) != 2)
    return NULL;
  if (ex->pair->value->type == tt_mention) {
    char *name = strdup(ex->pair->value->mention);
    SExpression *lambda = duplicate_expression(ex->pair->next->pair->value);
    //printf("Handling define:\n");
    //printf("Name is %s.\n", name);
    //printf("Args:"); print_expression(lambda->lambda->args);
    //printf("Body:"); print_expression(lambda->lambda->body);
    //printf("Arity = %d.\n", lambda->lambda->arity);
    ht_insert(ST, name, lambda);
  } else if (ex->pair->value->type == tt_pair) {
    SExpression *namewargs = ex->pair->value;
    SExpression *body = ex->pair->next->pair->value;
    SExpression *l = alloc_term(tt_lambda);
    char *name = strdup(namewargs->pair->value->mention);
    l->lambda->args = duplicate_expression(namewargs->pair->next);
    l->lambda->body = duplicate_expression(body);
    l->lambda->arity = list_length(l->lambda->args);
    ht_insert(ST, name, l);
  } else
    return NULL;
  return alloc_term(tt_nil);
}


SExpression *handle_if(SExpression *ex, SymbolTable *ST) {
  SExpression *predic = (ex->pair->value) ? duplicate_expression(ex->pair->value) : NULL,
    *ifbranch = (ex->pair->next->pair->value) ? duplicate_expression(ex->pair->next->pair->value) : NULL,
    *elsebranch = (ex->pair->next->pair->next) ? duplicate_expression(ex->pair->next->pair->next->pair->value) : NULL;
  printf("Handling if.\n");
  printf("Predicate:\t"); print_expression(predic);
  printf("If-branch:\t"); print_expression(ifbranch);
  printf("Else-branch:\t"); print_expression(elsebranch);
  if (!predic ||
      !ifbranch ||
      !elsebranch)
    return NULL;
  printf("Evaluating predicate.\n");
  predic = eval(predic, ST);
  if ((predic->type == tt_nil) ||
      ((predic->type == tt_bool) &&
       (predic->boolean == false))) {
    printf("Evaluating else-branch.\n");
    return eval(elsebranch, ST);
  } else {
    printf("Evaluating if-branch.\n");
    return eval(ifbranch, ST);
  }
}


SExpression *handle_eq(SExpression *ex, SymbolTable *ST) {
  SExpression *left = ex->pair->value, *right = ex->pair->next->pair->value;
  left = eval(left, ST);
  right = eval(right, ST);
  SExpression *answer = alloc_term(tt_bool);
  if (left->type != right->type)
    answer->boolean = false;
  else {
    switch (left->type) {
    case tt_bool:
      answer->boolean = (left->boolean == right->boolean) ? true : false;
      break;
    case tt_nil:
      answer->boolean = true;
      break;
    case tt_int:
      answer->boolean = (left->integer == right->integer) ? true : false;
      break;
    case tt_mention:
      answer->boolean = (strcmp(left->mention, right->mention) == 0) ? true : false;
      break;
    case tt_pair: // oh god, not this shit again
      answer = false;
    default:
      break;
    }
  }
  return answer;
}

  
SExpression *handle_mul(SExpression *ex, SymbolTable *ST) {
  if (!ex)
    return NULL;
  SExpression *result = alloc_term(tt_int);
  result->integer = 1;
  SExpression *current = ex, *tmp;
  while (current) {
    tmp = eval(current->pair->value, ST);
    if (tmp->type == tt_int)
      result->integer *= tmp->integer;
    else
      return NULL;
    current = current->pair->next;
  }
  return result;
}


SExpression *handle_plus(SExpression *ex, SymbolTable *ST) {
  if (!ex)
    return NULL;
  SExpression *result = alloc_term(tt_int);
  result->integer = 0;
  SExpression *current = ex, *tmp;
  while (current) {
    tmp = eval(current->pair->value, ST);
    if (tmp->type == tt_int)
      result->integer += tmp->integer;
    else
      return NULL;
    current = current->pair->next;
  }
  return result;
}


SExpression *handle_minus(SExpression *ex, SymbolTable *ST) {
  if (!ex ||
      ex->type != tt_pair ||
      ex->pair->value->type != tt_int)
    return NULL;
  SExpression *result = alloc_term(tt_int);
  result->integer = ex->pair->value->integer;
  if (!ex->pair->next)
    result->integer = -result->integer;
  else {
    SExpression *current = ex->pair->next, *tmp;
    while (current) {
      tmp = eval(current->pair->value, ST);
      if (tmp->type == tt_int)
	result->integer -= tmp->integer;
      else
	return NULL;
      current = current->pair->next;
    }
  }
  return result; 
}



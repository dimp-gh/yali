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
  //printf("Evaluating: "); print_expression(expr);
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
  

SExpression *substitute_all(SExpression *body, SExpression *args, SExpression *values, SymbolTable *ST) {
  SExpression *curr_arg = args;
  SExpression *curr_value = values;
  SExpression *curr_body = body;
  while(curr_arg &&
	curr_value &&
	curr_arg->pair->value &&
	curr_value->pair->value) {
    curr_body = substitute_mention(curr_body, 
				   curr_arg->pair->value->mention,
				   eval(curr_value->pair->value, ST));
    curr_arg = curr_arg->pair->next;
    curr_value = curr_value->pair->next;
  }
  return curr_body;
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
  //printf("Handling Apply.\n");
  //printf("Args: "); print_expression(function->lambda->args);
  //printf("Params: "); print_expression(args);
  //printf("Body: "); print_expression(function->lambda->body);
  if (!function || !args || !ST)
    return NULL;
  else if (function->lambda->arity == list_length(args)) {
    SExpression *subs = substitute_all(duplicate_expression(function->lambda->body),
				       function->lambda->args,
				       args,
				       ST);
    //printf("After substitution: "); print_expression(subs);
    return eval(subs, ST);
  } else
    return NULL;
}


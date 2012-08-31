#include <malloc.h>
#include <string.h>
#include "sexpr.h"
#include "common.h"


//#define DEBUG(X) printf("debug: %s.\n", X)
#define DEBUG(X)  


SExpression *eval(SExpression *expr, SymbolTable *ST) {
  /*
   * if SExpression is list AND
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
  else if (expr->type != tt_list) // if expression is one elementary-typed element
    return expr;
  else if (!expr->exprval) // if expression is empty list
    return NULL;
  else { // if expression is list and isn't empty
    SExpression *head = expr->exprval, *tail = head->next;
    if (head->type != tt_mention) // if head of list is not function call
      return NULL;
    char *call = head->mentval;
    // checking for special forms
    if (strcmp(call, "define") == 0)
      return handle_define(tail, ST);
    else if (strcmp(call, "if") == 0)
      return handle_if(tail, ST);
    else if (strcmp(call, "eq") == 0)
      return handle_eq(tail, ST);
    else if (strcmp(call, "dec") == 0)
      return handle_dec(tail, ST);
    else if (strcmp(call, "*") == 0)
      return handle_mul(tail, ST);
    // special forms check end
    else {
      Lambda *lambda = ht_lookup(ST, call);
      if (!lambda) // undefined name
	return NULL;
      SExpression *args = alloc_term(tt_list);
      args->exprval = duplicate_term(tail);
      return apply(lambda, args, ST);
    }
  }
}

SExpression *substitute_mention(SExpression *source, char *key, SExpression *value) {
  if (!source)
    return NULL;
  SExpression *current = source;
  while (current) {
    if ((current->type == tt_mention) &&
	(strcmp(current->mentval, key) == 0)) {
      switch (value->type) {
      case tt_int:
	current->type = tt_int;
	current->intval = value->intval;
	break;
      case tt_bool:
	current->type = tt_bool;
	current->boolval = value->boolval;
	break;
      case tt_nil:
	current->type = tt_nil;
	break;
      case tt_list:
	current->type = tt_list;
	current->exprval = duplicate_term(value); 
	break;
      default:
	break; 
      }
    } else if (current->type == tt_list)
      substitute_mention(current->exprval, key, value);
    current = current->next;
  }
  return source;
}
  

SExpression *apply(Lambda *lambda, SExpression *arglist, SymbolTable *ST) {
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
  //printf("Args: "); print_expr(lambda->args);
  //printf("Params: "); print_expr(arglist);
  //printf("Body: "); print_expr(lambda->body);
  SExpression *result = NULL;
  if ((lambda->arity == 0) && 
      (list_len(arglist) == 0))
     return eval(lambda->body, ST);
  else if ((list_len(arglist) == 1) &&
	   (lambda->arity == 1)) {
    SExpression *subs = substitute_mention(duplicate_term(lambda->body),
					   lambda->args->exprval->mentval,
					   eval(arglist->exprval, ST));
    //printf("After substitution: "); print_expr(subs);
    return eval(subs, ST);
  }
  return result;
}


SExpression *handle_define(SExpression *ex, SymbolTable *ST) {
  Lambda *lambda = alloc_lambda();
  SExpression *name = duplicate_term(ex),
    *args = duplicate_term(ex->next),
    *body = duplicate_term(ex->next->next);
  //printf("Handling define:\n");
  //printf("Name is %s.\n", name->mentval);
  //printf("Args:"); print_expr(args);
  //printf("Body:"); print_expr(body);
  lambda->args = args;
  lambda->body = body;
  lambda->arity = list_len(args);
  //printf("Arity = %d.\n", lambda->arity);
  ht_insert(ST, name->mentval, lambda);
  return alloc_term(tt_nil);
}


SExpression *handle_if(SExpression *ex, SymbolTable *ST) {
  SExpression *predic = (ex) ? duplicate_term(ex) : NULL,
    *ifbranch = (ex->next) ? duplicate_term(ex->next) : NULL,
    *elsebranch = (ex->next->next) ? duplicate_term(ex->next->next) : NULL;
  //  printf("Handling if.\n");
  //printf("Predicate:\t"); print_expr(predic);
  //printf("If-branch:\t"); print_expr(ifbranch);
  //printf("Else-branch:\t"); print_expr(elsebranch);
  if (!predic ||
      !ifbranch ||
      !elsebranch)
    return NULL;
  //printf("Evaluating predicate.\n");
  predic = eval(predic, ST);
  if ((predic->type == tt_nil) ||
      ((predic->type == tt_bool) &&
       (predic->boolval == false))) {
    //printf("Evaluating else-branch.\n");
    return eval(elsebranch, ST);
  } else {
    //printf("Evaluating if-branch.\n");
    return eval(ifbranch, ST);
  }
}


SExpression *handle_eq(SExpression *ex, SymbolTable *ST) {
  SExpression *left = ex, *right = ex->next;
  left = eval(left, ST);
  right = eval(right, ST);
  SExpression *answer = alloc_term(tt_bool);
  if (left->type != right->type)
    answer->boolval = false;
  else {
    switch (left->type) {
    case tt_bool:
      answer->boolval = (left->boolval == right->boolval) ? true : false;
      break;
    case tt_nil:
      answer->boolval = true;
      break;
    case tt_int:
      answer->boolval = (left->intval == right->intval) ? true : false;
      break;
    case tt_mention:
      answer->boolval = (strcmp(left->mentval, right->mentval) == 0) ? true : false;
      break;
    case tt_list: // oh god, not this shit again
      answer = false;
    default:
      break;
    }
  }
  return answer;
}

SExpression *handle_dec(SExpression *ex, SymbolTable *ST) {
  if (!ex)
    return NULL;
  else if (ex->type != tt_int)
    ex = eval(ex, ST);
  if (ex->type != tt_int)
    return NULL;
  SExpression *result = alloc_term(tt_int);
  result->intval = ex->intval - 1;
  return result;
}
    
SExpression *handle_mul(SExpression *ex, SymbolTable *ST) {
  if (!ex)
    return NULL;
  SExpression *left = ex, *right = ex->next;
  if (!right)
    return NULL;
  left = eval(left, ST);
  right = eval(right, ST);
  if ((left->type != tt_int) ||
      (right->type != tt_int))
    return NULL;
  else {
    SExpression *result = alloc_term(tt_int);
    result->intval = left->intval * right->intval;
    return result;
  }
}

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "corelib.h"
#include "common.h"


SymbolTable *CoreLibrary = NULL;
extern SymbolTable *UserLibrary;


SExpression *handle_define(SExpression *args) {
  if (!args ||
      list_length(args) != 2 ||
      !UserLibrary)
    return NULL;
  SExpression *func = NULL;
  char *name;
  if (args->pair->value->type == tt_mention) {
    name = strdup(args->pair->value->mention);
    func = duplicate_expression(args->pair->next->pair->value);
  } else if (args->pair->value->type == tt_pair) {
    SExpression *namewargs = args->pair->value;
    SExpression *body = args->pair->next->pair->value;
    func = alloc_term(tt_lambda);
    name = strdup(namewargs->pair->value->mention);
    func->lambda->args = duplicate_expression(namewargs->pair->next);
    func->lambda->body = duplicate_expression(body);
    func->lambda->arity = list_length(func->lambda->args); 
  } else
    return NULL;
  //printf("Handling define:\n");
  //printf("Name is %s.\n", name);
  //printf("Args:"); print_expression(l->lambda->args);
  //printf("Body:"); print_expression(l->lambda->body);
  //printf("Arity = %d.\n", l->lambda->arity);
  ht_insert(UserLibrary, name, func);
  return alloc_term(tt_nil);
}


SExpression *handle_if(SExpression *args) {
  SExpression *predic = (args->pair->value) ? duplicate_expression(args->pair->value) : NULL,
    *ifbranch = (args->pair->next->pair->value) ? duplicate_expression(args->pair->next->pair->value) : NULL,
    *elsebranch = (args->pair->next->pair->next) ? duplicate_expression(args->pair->next->pair->next->pair->value) : NULL;
  //printf("Handling if.\n");
  //printf("Predicate:\t"); print_expression(predic);
  //printf("If-branch:\t"); print_expression(ifbranch);
  //printf("Else-branch:\t"); print_expression(elsebranch);
  if (!predic ||
      !ifbranch ||
      !elsebranch)
    return NULL;
  //printf("Evaluating predicate.\n");
  predic = eval(predic);
  if ((predic->type == tt_nil) ||
      ((predic->type == tt_bool) &&
       (predic->boolean == false))) {
    //printf("Evaluating else-branch.\n");
    return eval(elsebranch);
  } else {
    //printf("Evaluating if-branch.\n");
    return eval(ifbranch);
  }
}

// Comparison functions

SExpression *handle_equal(SExpression *args) {
  if (!args)
    return NULL;
  SExpression *left = args->pair->value, *right = args->pair->next->pair->value;
  left = eval(left);
  right = eval(right);
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
    case tt_lambda:
      answer = false;
    default:
      break;
    }
  }
  return answer;
}


SExpression *handle_less(SExpression *args) {
  if (!args)
    return NULL;
  SExpression *left = args->pair->value, *right = args->pair->next->pair->value;
  left = eval(left);
  right = eval(right);
  SExpression *answer = alloc_term(tt_bool);
  if (left->type != right->type &&
      left->type != tt_int)
    answer->boolean = false;
  else 
    answer->boolean = (left->integer < right->integer) ? true : false;
  return answer;
}


SExpression *handle_greater(SExpression *args) {
  if (!args)
    return NULL;
  SExpression *left = args->pair->value, *right = args->pair->next->pair->value;
  left = eval(left);
  right = eval(right);
  SExpression *answer = alloc_term(tt_bool);
  if (left->type != right->type &&
      left->type != tt_int)
    answer->boolean = false;
  else 
    answer->boolean = (left->integer > right->integer) ? true : false;
  return answer;
}

// End of comparison functions.
// Logic functions.


SExpression *handle_and(SExpression *args) {
  if (!args)
    return NULL;
  SExpression *current = args, *tmp;
  SExpression *result = alloc_term(tt_bool);
  result->boolean = true;
  while (current) {
    tmp = eval(current->pair->value);
    if ((tmp->type == tt_bool && tmp->boolean == false) ||
	(tmp->type == tt_nil)) {
      result->boolean = false;
      break;
    }
    current = current->pair->next;
  }
  return result;
}


SExpression *handle_or(SExpression *args) {
  if (!args)
    return NULL;
  SExpression *current = args, *tmp;
  while (current) {
    tmp = eval(current->pair->value);
    if ((tmp->type == tt_bool && tmp->boolean == false) ||
	(tmp->type == tt_nil))
      current = current->pair->next;
    else
      return tmp;
  }
  SExpression *false_res = alloc_term(tt_bool);
  false_res->boolean = false;
  return false_res;
}


// End of logic functions.
// Arithmetic functions.
  
SExpression *handle_mult(SExpression *args) {
  if (!args)
    return NULL;
  SExpression *result = alloc_term(tt_int);
  result->integer = 1;
  SExpression *current = args, *tmp;
  while (current) {
    tmp = eval(current->pair->value);
    if (tmp->type == tt_int)
      result->integer *= tmp->integer;
    else
      return NULL;
    current = current->pair->next;
  }
  return result;
}


SExpression *handle_plus(SExpression *args) {
  if (!args)
    return NULL;
  SExpression *result = alloc_term(tt_int);
  result->integer = 0;
  SExpression *current = args, *tmp;
  while (current) {
    tmp = eval(current->pair->value);
    if (tmp->type == tt_int)
      result->integer += tmp->integer;
    else
      return NULL;
    current = current->pair->next;
  }
  return result;
}


SExpression *handle_minus(SExpression *args) {
  if (!args ||
      args->type != tt_pair)
    return NULL;
  SExpression *result = alloc_term(tt_int);
  result->integer = eval(args->pair->value)->integer;
  if (!args->pair->next)
    result->integer = -result->integer;
  else {
    SExpression *current = args->pair->next, *tmp;
    while (current) {
      tmp = eval(current->pair->value);
      if (tmp->type == tt_int)
	result->integer -= tmp->integer;
      else
	return NULL;
      current = current->pair->next;
    }
  }
  return result; 
}


SExpression *handle_divide(SExpression *args) {
  if (!args ||
      args->type != tt_pair)
    return NULL;
  int dividend = eval(args->pair->value)->integer;
  SExpression *current = args->pair->next, *tmp;
  while (current) {
    tmp = eval(current->pair->value);
    if (tmp->type == tt_int)
      dividend /= tmp->integer;
    else
      return NULL;
    current = current->pair->next;
  }
  SExpression *result = alloc_term(tt_int);
  result->integer = dividend;
  return result;
}


SExpression *handle_remainder(SExpression *args) {
  if (!args ||
      args->type != tt_pair)
    return NULL;
  int dividend = eval(args->pair->value)->integer;
  int divisor = eval(args->pair->next->pair->value)->integer;
  SExpression *result = alloc_term(tt_int);
  result->integer = dividend % divisor;
  return result; 
}

// End of arithmetic functions.
// List functions


SExpression *handle_car(SExpression *arg) {
  if (!arg ||
      arg->type != tt_pair)
    return NULL;
  SExpression *evald =  eval(arg->pair->value);
  return evald->pair->value;
}


SExpression *handle_cdr(SExpression *arg) {
  if (!arg ||
      arg->type != tt_pair)
    return NULL;
  SExpression *list = eval(arg->pair->value);
  return (list->pair->next) ? list->pair->next : alloc_term(tt_nil);
}


SExpression *handle_list(SExpression *args) {
  if (!args ||
      args->type != tt_pair)
    return NULL;  
  SExpression *current = args, *tmp;
  SExpression *root = NULL, *last = NULL;
  while (current) {
    tmp = eval(current->pair->value);
    if (!root)
      last = root = alloc_term(tt_pair);
    else
      last = last->pair->next = alloc_term(tt_pair);
    last->pair->value = tmp;
    current = current->pair->next;
  }
  return root;
}

// End of list functions.

void load_core_library() {
  if (CoreLibrary)
    return;
  CoreLibrary = ht_create(32);
  // Comparison.
  ht_insert(CoreLibrary, "=", handle_equal);
  ht_insert(CoreLibrary, ">", handle_greater);
  ht_insert(CoreLibrary, "<", handle_less);
  // Logic.
  ht_insert(CoreLibrary, "or", handle_or);
  ht_insert(CoreLibrary, "and", handle_and);
  // Arithmetic.
  ht_insert(CoreLibrary, "+", handle_plus);
  ht_insert(CoreLibrary, "-", handle_minus);
  ht_insert(CoreLibrary, "*", handle_mult);
  ht_insert(CoreLibrary, "div", handle_divide);
  ht_insert(CoreLibrary, "rem", handle_remainder);
  // Conditions.
  ht_insert(CoreLibrary, "if", handle_if);
  // Define.
  ht_insert(CoreLibrary, "define", handle_define);
  // Loops?
  // I/O?
  // Random?
  // Lists.
  ht_insert(CoreLibrary, "car", handle_car);
  ht_insert(CoreLibrary, "cdr", handle_cdr);
  ht_insert(CoreLibrary, "list", handle_list);
  // Type predicates?
}


void *find_core_function(char *mention) {
  if (!mention || 
      !CoreLibrary)
    return NULL;
  void *result = ht_lookup(CoreLibrary, mention);
  return result;
}


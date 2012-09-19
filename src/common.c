#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "common.h"

#define TYPE(MARKER) (withtypes) ? MARKER : ""

void _print_expression(SExpression *expr, int withtypes) {
  const char *boolrepr[] = {"#f", "#t"};
  if (expr) {
    switch (expr->type) {
    case tt_int:
      printf("%s%ld", TYPE("int:"), expr->integer);
      break;
    case tt_float:
      printf("%s%f", TYPE("float:"), expr->real);
      break;
    case tt_string:
      printf("%s'%s'", TYPE("string:"), expr->string);
      break;
    case tt_nil:
      printf("%s#nil", TYPE("nil:"));
      break;
    case tt_bool:
      printf("%s%s", TYPE("bool:"), boolrepr[expr->boolean]);
      break;
    case tt_mention:
      printf("%s%s", TYPE("mention:"), expr->mention);
      break;
    case tt_lambda:
      printf("%s(lambda ", TYPE("lambda:"));
      _print_expression(expr->lambda->args, withtypes);
      printf(" ");
      _print_expression(expr->lambda->body, withtypes);
      printf(")");
      break;
    case tt_pair:
      printf("%s(", TYPE("list:"));
      SExpression *temp = expr;
      while (temp) {
	_print_expression(temp->pair->value, withtypes);
	if (temp->pair->next) {
	  printf(" ");
	  temp = temp->pair->next;
	} else
	  break;
      }
      printf(")");
      break;
    default:
      printf("Unknown(type=%d)", expr->type);
      break;
    }
  } else
    printf("Null");
}


void print_expression(SExpression *root) {
  _print_expression(root, 0);
  printf("\n");
}


void print_typed_expression(SExpression *root) {
  _print_expression(root, 1);
  printf("\n");
}


Lambda *alloc_lambda() {
  Lambda *lamb = malloc(sizeof(Lambda));
  lamb->arity = 0;
  lamb->args = NULL;
  lamb->body = NULL;
  return lamb;
}


List *alloc_pair() {
  List *pair = malloc(sizeof(List));
  pair->value = NULL;
  pair->next = NULL;
   return pair;
}


SExpression *alloc_term(enum Term_type type) {
  SExpression *res = malloc(sizeof(SExpression));
  res->type = type;
  switch (type) {
  case tt_nil:
    break;
  case tt_int:
    res->integer = 0;
    break;
  case tt_float:
    res->real = 0;
    break;
  case tt_string:
    res->string = NULL;
    break;
  case tt_bool:
    res->boolean = false;
    break;
  case tt_lambda:
    res->lambda = alloc_lambda();
    break;
  case tt_pair:
    res->pair = alloc_pair();
    break;
  case tt_mention:
    res->mention = NULL;
    break;
  default:
    break;
  }     
  return res;
}


void dealloc_lambda(Lambda *lambda) {
  if (!lambda)
    return;
  dealloc_expression(lambda->args);
  dealloc_expression(lambda->body);
  free(lambda);
}


void dealloc_expression(SExpression *expr) {
  if (!expr)
    return;
  switch (expr->type) {
  case tt_nil:
  case tt_int:
  case tt_float:
  case tt_bool:
    free(expr);
    break;
  case tt_mention:
    free(expr->mention);
    free(expr);
    break;
  case tt_string:
    free(expr->string);
    free(expr);
    break;
  case tt_pair: {   
    SExpression *current = expr;
    SExpression *tmp;
    while (current) {
      dealloc_expression(current->pair->value);
      tmp = current->pair->next;
      free(current);
      current = tmp;
    }
    break;
  }
  case tt_lambda:
    dealloc_lambda(expr->lambda);
    free(expr);
    break;
  default: 
    break;
  }
}


SExpression *duplicate_expression(SExpression *ex) {
  if (!ex)
    return NULL;
  SExpression *result = NULL;
  switch (ex->type) {
  case tt_int:
    result = alloc_term(tt_int);
    result->integer = ex->integer;
    break;
  case tt_float:
    result = alloc_term(tt_float);
    result->real = ex->real;
    break;
  case tt_string:
    result = alloc_term(tt_string);
    result->string = strdup(ex->string);
    break;
  case tt_mention:
    result = alloc_term(tt_mention);
    result->mention = strdup(ex->mention);
    break;
  case tt_bool:
    result = alloc_term(tt_bool);
    result->boolean = ex->boolean;
    break;
  case tt_nil:
    result = alloc_term(tt_nil);
    break;
  case tt_pair:
    result = alloc_term(tt_pair);
    result->pair->value = duplicate_expression(ex->pair->value);
    result->pair->next = duplicate_expression(ex->pair->next);
    break;
  case tt_lambda:
    result = alloc_term(tt_lambda);
    result->lambda->arity = ex->lambda->arity;
    result->lambda->args = duplicate_expression(ex->lambda->args);
    result->lambda->body = duplicate_expression(ex->lambda->body);
    break;
  default:
    break; 
  }
  return result;
}


int list_length(SExpression *list) {
  if (!list)
    return 0;
  else if (list->type != tt_pair)
    return -1;
  else {
    SExpression *current = list;
    int len = 0;
    while (current) {
      if (current->type == tt_pair)
	current = current->pair->next;
      else
	break;
      len++;
    }
    return len;
  }
}


void report_error(char *format, ...) {
  va_list argptr;
  va_start(argptr, format);
  fprintf(stderr, "Error: ");
  vfprintf(stderr, format, argptr);
  va_end(argptr);
  fprintf(stderr, "\n");
}

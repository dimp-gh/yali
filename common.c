#include "common.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>


#define TYPE(MARKER) (withtypes) ? MARKER : ""

void _print_expression(SExpression *expr, int withtypes) {
  const char *boolrepr[] = {"#f", "#t"};
  if (expr) {
    switch (expr->type) {
    case tt_int:
      printf("%s%ld", TYPE("int:"), expr->integer);
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
  }
}


void print_expression(SExpression *root) {
  _print_expression(root, 0);
  printf(".\n");
}

void print_typed_expression(SExpression *root) {
  _print_expression(root, 1);
  printf(".\n");
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


void dealloc_lambda(Lambda *lambda) {
  if (!lambda)
    return;
  dealloc_expr(lambda->args);
  dealloc_expr(lambda->body);
  free(lambda);
}

void dealloc_expr(SExpression *expr) {
  if (!expr)
    return;
  switch (expr->type) {
  case tt_nil:
  case tt_int:
  case tt_bool:
    free(expr);
    break;
  case tt_mention:
    free(expr->mention);
    free(expr);
    break;
  case tt_lambda:
    dealloc_lambda(expr->lambda);
    free(expr);
    break;
  case tt_pair:
    SExpression *current = expr, *tmp;
    while (current) {
      dealloc_expr(current->pair->value);
      tmp = current->pair->next;
      free(current);
      current = tmp;
    }
    break;
  default:
    break;
  }
}

SExpression *duplicate_term(SExpression *ex) {
  if (!ex)
    return NULL;
  SExpression *result = NULL;
  switch (ex->type) {
  case tt_int:
    result = alloc_term(tt_int);
    result->intval = ex->intval;
    break;
  case tt_mention:
    result = alloc_term(tt_mention);
    result->mentval = strdup(ex->mentval);
    break;
  case tt_bool:
    result = alloc_term(tt_bool);
    result->boolval = ex->boolval;
    break;
  case tt_nil:
    result = alloc_term(tt_nil);
    break;
  case tt_list:
    result = duplicate_list(ex);
    break;
  default:
    break; 
  }
  return result;
}

SExpression *duplicate_list(SExpression *list) {
  SExpression *root = alloc_term(tt_list), *head = list->exprval;
  if (head) {
    root->exprval = duplicate_term(head);
    SExpression *last = root->exprval, *current = head->next;
    while (current) {
      last->next = duplicate_term(current);
      last = last->next;
      current = current->next;
    }
  }
  return root;
}


int list_len(SExpression *list) {
  if ((!list) ||
      (list->type != tt_list))
    return -1;
  else {
    SExpression *current = list->exprval;
    int len = 0;
    while (current) {
      current = current->next;
      len++;
    }
    return len;
  }
}
      

Lambda *create_lambda(SExpression *root) {
  SExpression *head, *args, *body;
  if ((!root) ||
      (root->type != tt_list) ||
      (!(head = root->exprval)) ||
      (head->type != tt_mention) ||
      (strcmp(head->mentval, "lambda") != 0) ||
      (!(args = head->next)) ||
      (args->type != tt_list) ||
      (!(body = args->next)) ||
      (body-> type != tt_list))
    return NULL;
  else {
    Lambda *lambda = alloc_lambda();
    lambda->args = duplicate_term(args);
    lambda->body = duplicate_term(body);
    lambda->arity = list_len(args);
    return lambda;
  }
}     
*/

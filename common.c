#include "common.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
//#include <ctype.h>


//#define DEBUG(X) printf("debug: %s.\n", X)
#define DEBUG(X)  


void _print_expr(SExpression *expr, int with_types) {
  const char boolrepr[2][3] = {"#f", "#t"};
  DEBUG("Entering _print_sexp()");
  if (expr) {
    DEBUG("Expr is not null, looking for it's type");
    switch (expr->type) {
    case tt_int:
      DEBUG("It's integer, printing");
      printf("%s%ld", (with_types) ? "int::" : "", expr->intval);
      break;
    case tt_nil:
      DEBUG("It's nil, printing");
      printf("%s#nil", (with_types) ? "nil::" : "");
      break;
    case tt_list:
      DEBUG("It's list. Printing open paren. Then printing list. Then closing paren");
      printf("%s(", (with_types) ? "list::" : "");
      _print_expr(expr->exprval, with_types);
      printf(")");
      break;
    case tt_bool:
      DEBUG("It's bool. Printing");
      printf("%s%s", (with_types) ? "bool::" : "", boolrepr[expr->boolval]);
      break;
    case tt_mention:
      printf("%s%s", (with_types) ? "ment::" : "", expr->mentval);
      break;
    default:
      DEBUG("It has unknown type. Printing");
      printf("Unknown(%d)", expr->type);
      break;
    }
    DEBUG("Ended printing current term");
    if (expr->next) {
      DEBUG("We've got the next term - start printing it");
      printf(" ");
      _print_expr(expr->next, with_types);
    }
  }
  DEBUG("Leaving _print_sexp()");
}


void print_expr(SExpression *s) {
  _print_expr(s, 0);
  printf(".\n");
}


void print_typed_expr(SExpression *s) {
  _print_expr(s, 1);
  printf(".\n");
}


SExpression *alloc_term(enum Term_type type) {
  SExpression *res = malloc(sizeof(SExpression));
  res->type = type;
  res->next = NULL;
  return res;
}

Lambda *alloc_lambda() {
  Lambda *lamb = malloc(sizeof(Lambda));
  lamb->arity = 0;
  lamb->args = NULL;
  lamb->body = NULL;
  return lamb;
}

void dealloc_expr(SExpression *expr) {
  if (!expr)
    return;
  SExpression *current = expr, *next;
  do {
    next = current->next;
    switch (current->type) {
    case tt_list:
      dealloc_expr(current->exprval);
      break;
    case tt_mention:
      free(current->mentval);
      break;
    default:
      break;
    }
    free(current);
    current = next;
  } while (current);
}

void dealloc_lambda(Lambda *lambda) {
  if (!lambda)
    return;
  dealloc_expr(lambda->args);
  dealloc_expr(lambda->body);
  free(lambda);
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

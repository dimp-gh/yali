#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sexpr.h"
#include "common.h"
#include "lexer.h"


#define DEBUG(X)  

#define UPDATE_ROOT_LAST(R, L, N) {			\
  if (!R->pair->value) {				\
    R->pair->value = N;					\
    L = R;						\
  } else {						\
    L = L->pair->next = alloc_term(tt_pair);		\
    L->pair->value = N;					\
  }							\
}


Token_node *_parse_expression(Token_node *token_head, SExpression *root) {
  if (!token_head)
    return NULL;
  SExpression *last = root, *new;
  Token_node *current = token_head;
  int done = 0;
  while (current && !done) {
    switch(current->token->type) {
    case token_nil:
      new = alloc_term(tt_nil);
      UPDATE_ROOT_LAST(root, last, new);
      break;
    case token_integer:
      new = alloc_term(tt_int);
      new->integer = atol(current->token->strval);
      UPDATE_ROOT_LAST(root, last, new);
      break;
    case token_bool:
      new = alloc_term(tt_bool);
      new->boolean = (strcmp(current->token->strval, "#t") == 0) ? true : false;
      UPDATE_ROOT_LAST(root, last, new);
      break;
    case token_close_paren:
      done = 1;
      return current;
      break;
    case token_open_paren:
      new = alloc_term(tt_pair);
      current = _parse_expression(current->next, new);
      UPDATE_ROOT_LAST(root, last, new);
      break;
    case token_id:
      if (strcmp(current->token->strval, "lambda") != 0) {
	new = alloc_term(tt_mention);
	new->mention = strdup(current->token->strval);
	UPDATE_ROOT_LAST(root, last, new);
      } else {
	new = alloc_term(tt_lambda);
	SExpression *args, *body;
	args = alloc_term(tt_pair);
	current = _parse_expression(current->next->next, args);
	body = alloc_term(tt_pair);
	current = _parse_expression(current->next->next, body);
	new->lambda->args = args;
	new->lambda->body = body;
	UPDATE_ROOT_LAST(root, last, new);
      }
      break;
    default:
      printf("Unknown token, alert!\n");
      break;
    }
    current = current->next;
  }
  return current;
}


void _print_expression(SExpression *expr) {
  const char *boolrepr[] = {"#f", "#t"};
  if (expr) {
    switch (expr->type) {
    case tt_int:
      printf("%ld", expr->integer);
      break;
    case tt_nil:
      printf("#nil");
      break;
    case tt_bool:
      printf("%s", boolrepr[expr->boolean]);
      break;
    case tt_mention:
      printf("%s", expr->mention);
      break;
    case tt_lambda:
      printf("(lambda ");
      _print_expression(expr->lambda->args);
      printf(" ");
      _print_expression(expr->lambda->body);
      printf(")");
      break;
    case tt_pair:
      printf("(");
      SExpression *temp = expr;
      while (temp) {
	_print_expression(temp->pair->value);
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

SExpression *parse(Token_node *head) {
  Token_node *current = head;
  SExpression *result = NULL;
  while (current) {
    if (current->token->type == token_open_paren) {
      result = alloc_term(tt_pair);
      current = _parse_expression(current->next, result);
      return result;
    }
    current = current->next;
  }    
  return result;
}


void print_expression(SExpression *expr) {
  _print_expression(expr);
  printf(".\n");
}

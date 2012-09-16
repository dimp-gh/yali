#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sexpr.h"
#include "common.h"
#include "lexer.h"
#include "parser.h"


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
    case token_float:
      new = alloc_term(tt_float);
      new->real = atof(current->token->strval);
      UPDATE_ROOT_LAST(root, last, new);
      break;
    case token_string:
      new = alloc_term(tt_string);
      new->string = strdup(current->token->strval);
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
      if (new->pair->value->type == tt_mention &&
	  strcmp(new->pair->value->mention, "lambda") == 0) {
	SExpression *l = alloc_term(tt_lambda);
	l->lambda->args = new->pair->next->pair->value;
	l->lambda->body = new->pair->next->pair->next->pair->value;
	l->lambda->arity = list_length(l->lambda->args);
	UPDATE_ROOT_LAST(root, last, l);
      } else
	UPDATE_ROOT_LAST(root, last, new);
      break;
    case token_id:
      new = alloc_term(tt_mention);
      new->mention = strdup(current->token->strval);
      UPDATE_ROOT_LAST(root, last, new);
      break;
    default:
      printf("Unknown token, alert!\n");
      break;
    }
    current = current->next;
  }
  return current;
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

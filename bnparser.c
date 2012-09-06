#include <string.h>
#include "sexpr.h"
#include "lexer.h"


#define UPDATE_HEAD_LAST(H, L, N) {             \
 if (!H)					\
   L = H = N;					\
 else						\
   L = L->next = N;				\
}


Token_node *parse_expression(Token_node *token_head, SExpression *root) {
  if (!token_head)
    return NULL;
  SExpression *last = root, *new;
  Token_node *current = token_head;
  int done = 0;
  while (current && !done) {
    switch(current->token->type) {
    case token_nil:
      new = alloc_term(tt_nil);
      UPDATE_HEAD_LAST(root, last, new);
      break;
    case token_integer:
      new = alloc_term(tt_int);
      new->intval = atol(current->token->strval);
      UPDATE_HEAD_LAST(root, last, new);
      break;
    case token_bool:
      new = alloc_term(tt_bool);
      new->boolval = (strcmp(current->token->strval, "#t") == 0) ? true : false;
      UPDATE_HEAD_LAST(root, last, new);
      break;
    case token_close_paren:
      done = 1;
      break;
    case token_open_paren:
      new = alloc_term(tt_pair);
      current = parse_expression(current->next, new);
      UPDATE_HEAD_LAST(root, last, new);
      break;
    case token_id:
      if (strcmp(current->token->strval, "lambda") != 0) {
	new = alloc_term(tt_mention);
	new->mentval = strdup(current->token->strval);
	UPDATE_HEAD_LAST(root, last, new);
      } else {
	new = alloc_term(tt_pair);
	SExpression *args, *body;
	new->pair->value = args = alloc_term(tt_pair);
	current = parse_expression(current->next->next, args);
	args->pair->next = body = alloc_term(tt_pair);
	current = parse_expression(current->next->next, body);
	UPDATE_HEAD_LAST(root, last, new);
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



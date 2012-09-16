#include <ctype.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"


int is_bool(char* c) {
  return ((strcmp(c, "#t") == 0) || (strcmp(c, "#f") == 0));
}

int is_nil(char *c) {
  return strcmp(c, "#nil") == 0;
}

int is_int(char *c) {
  char *current = c;
  while (*current != '\0') {
    if (!isdigit(*current))
      return 0;
    current++;
  }
  return 1;
}

int is_float(char *c) {
  int dot_count = 0;
  char *current = c;
  while (*current != '\0') {
    if (*current == '.')
      dot_count++;
    else if (!isdigit(*current))
      return 0;
    current++;
  }
  return dot_count == 1;
}

int is_whitespace(char *c) {
  return isspace(*c);
}

int is_paren(char *c) {
  return *c == '(' || *c == ')';
}

int is_comment_mark(char *c) {
  return *c == ';';
}

int is_quote(char *c) {
  return *c == '\'';
}


Token_node *alloc_token(enum Token_type type) {
  Token_node *res = malloc(sizeof(Token_node));
  res->next = NULL;
  res->token = malloc(sizeof(Token));
  res->token->type = type;
  res->token->strval = NULL;
  return res;
}


enum Token_type guess_type(char *str) {
  if (is_nil(str))
    return token_nil;
  else if (is_bool(str))
    return token_bool;
  else if (is_float(str))
    return token_float;
  else if (is_int(str))
    return token_integer;
  else
    return token_id;
}


#define UPDATE_HEAD_LAST(H, L, N) {             \
 if (!H)					\
   L = H = N;					\
 else						\
   L = L->next = N;				\
}


Token_node *lex(char *text) {
  enum Lex_state state = lex_reading;
  char buffer[256], *pbuf = buffer;
  Token_node *head = NULL, *last = NULL;
  char *current = text;
  while ((current) && (*current != '\0') && (*current != EOF)) {
    switch(state) {
    case lex_in_comment:
      if (*current == '\n')
	state = lex_reading;
      break;
    case lex_in_string:
      if (is_quote(current)) {
	*pbuf = '\0';
	Token_node *str_token = alloc_token(token_string);
	str_token->token->strval = strdup(buffer);
	UPDATE_HEAD_LAST(head, last, str_token);
	state = lex_reading;
      } else
	*pbuf++ = *current;
      break;
    case lex_in_token:
      if (is_paren(current)) {
	// Add buffer as token_id to tokens.
	*pbuf = '\0';
	Token_node *mention = alloc_token(guess_type(buffer));
	mention->token->strval = strdup(buffer);
	UPDATE_HEAD_LAST(head, last, mention);
	// Add paren token to tokens.
	Token_node *paren = alloc_token((*current == '(') ?
					token_open_paren :
					token_close_paren);
	UPDATE_HEAD_LAST(head, last, paren);
	// Switch state to lex_reading.
	state = lex_reading;
      } else if (is_comment_mark(current)) {
	// Add buffer as token_id to tokens.
	*pbuf = '\0';
	Token_node *mention = alloc_token(guess_type(buffer));
	mention->token->strval = strdup(buffer);
	UPDATE_HEAD_LAST(head, last, mention);
	// switch to state lex_in_comment
	state = lex_in_comment;
      } else if (is_whitespace(current)) {
	// Add buffer as token_id to tokens.
	*pbuf = '\0';
	Token_node *mention = alloc_token(guess_type(buffer));
	mention->token->strval = strdup(buffer);
	UPDATE_HEAD_LAST(head, last, mention); 
	// switch to state lex_reading
	state = lex_reading;
      } else
	// accumulate token in buffer
	*pbuf++ = *current;
      break;
    case lex_reading:
      if (is_paren(current)) {
	// add paren token to tokens.
	Token_node *paren = alloc_token((*current == '(') ?
					token_open_paren :
					token_close_paren);
	UPDATE_HEAD_LAST(head, last, paren);
      } else if (is_comment_mark(current)) {
	// switch state to comment
	state = lex_in_comment;
      } else if (is_whitespace(current)) {
	// skip whitespace, do nothing
	;
      } else if (is_quote(current)) {
	// switch state to lex_in_string
	state = lex_in_string;
	pbuf = buffer;
      } else {
	// We've got not whitespace, nor paren, nor comment, nor quote character.
	// Clear buffer, add current char to buffer.
	pbuf = buffer;
	*pbuf++ = *current;
	// Switch state to lex_in_token
	state = lex_in_token;
      }
      break;     
    }
    current++;
  }
  return head;
}


void _print_tokens(Token_node *start) {
  if (!start)
    printf("Empty token stream.\n");
  else {
    Token_node *current = start;
    do {
      switch(current->token->type) {
      case token_open_paren:
	printf("open_paren:( ");
	break;
      case token_close_paren:
	printf("close_paren:) ");
	break;
      case token_id:
	printf("id:%s ", current->token->strval);
	break;
      case token_integer:
	printf("integer:%s ", current->token->strval);
	break;
      case token_float:
	printf("float:%s ", current->token->strval);
	break;
      case token_bool:
	printf("bool:%s ", current->token->strval);
	break;
      case token_string:
	printf("string:'%s' ", current->token->strval);
	break;
      case token_nil:
	printf("nil:%s ", current->token->strval);
	break;
      default:
	printf("<unknown_token>:%s ", current->token->strval);
	break;
      }
      current = current->next;
    } while(current);
    printf(".\n");
  }
}    
    

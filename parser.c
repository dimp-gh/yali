#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "common.h"


int is_mention(char c) {
  return (c >= 'a' && c <= 'z') || (c >= '!' && c <= '/') || (c == '_');
}


int is_bool_or_nil(char c) {
  return c == '#';
}


int is_comment(char c) {
  return c == ';';
}


int is_int(char c) {
  return c >= '0' && c <= '9';
}


SExpression *create_term_from_repr(char *repr) {
  SExpression *result = NULL;
  if (is_mention(*repr)) {
    result = alloc_term(tt_mention);
    result->mentval = strdup(repr);
  } else if (is_int(*repr)) {
    result = alloc_term(tt_int);
    result->intval = atol(repr);
  } else if (is_bool_or_nil(*repr)) {
    if (*(repr+1) == 'n')
      result = alloc_term(tt_nil);
    else {
      result = alloc_term(tt_bool);
      result->boolval = (*(repr+1) == 't') ? true : false;
    }
  }
  return result;		       
}


char *parse_expr(char *stream, SExpression *expr) {
  char buffer[32];
  char *curr = stream, *buf = &buffer[0];    
  SExpression *last = NULL;
  int done = 0;
  while (!done && (*curr != '\0')) {
    switch (*curr) {
    case ')': {
      *buf = '\0';
      SExpression *tmp = create_term_from_repr(&buffer[0]);
      if (last) {
	last->next = tmp;
	last = tmp;
      } else
	expr->exprval = last = tmp;
      buf = &buffer[0];
      return curr+1;
      break;
    }
    case ' ': {
      *buf = '\0';
      SExpression *tmp = create_term_from_repr(&buffer[0]);
      if (last) {
	last->next = tmp;
	last = tmp;
      } else
	expr->exprval = last = tmp;
      buf = &buffer[0];
      break;
    }
    case '(': {
      *buf = '\0';
      SExpression *tmp = alloc_term(tt_list);
      curr = parse_expr(curr+1, tmp);
      if (last) {
	last->next = tmp;
	last = last->next;
      } else
	expr->exprval = last = tmp;
      buf = &buffer[0];
      break;
    }
    default:
      *buf = *curr;
      buf++;
      break;
    }
    curr++;
  }
  return curr;
}


SExpression *parse(char *stream) {
  SExpression *root = NULL, *last = NULL;
  if (!stream)
    return NULL;
  while (*stream != '\0') {
    if (*stream == '(') {
      SExpression *tmp = alloc_term(tt_list);
      stream = parse_expr(++stream, tmp);
      if (!tmp) {
	dealloc_expr(root);
	return NULL;
      }
      if (!root) {
	last = root = tmp;
      } else {
	last->next = tmp;
	last = tmp;
      }
    } else
      stream++;
  }
  return root;
}

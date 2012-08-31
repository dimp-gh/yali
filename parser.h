#include "sexpr.h"
#ifndef PARSER
#define PARSER


SExpression *create_term_from_repr(char *repr);
char *parse_expr(char *stream, SExpression *expr);
SExpression *parse(char *stream);

#endif

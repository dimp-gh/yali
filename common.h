#include "sexpr.h"
#ifndef COMMON
#define COMMON

void _print_expr(SExpression *expr, int with_types);
void print_expr(SExpression *expr);
void print_typed_expr(SExpression *expr);
SExpression *alloc_term(enum Term_type type);
void dealloc_expr(SExpression *term); 

Lambda *alloc_lambda();
SExpression *duplicate_term(SExpression *term);
SExpression *duplicate_list(SExpression *head);
int list_len(SExpression *list);
#endif

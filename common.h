#include "sexpr.h"
#ifndef COMMON
#define COMMON

void print_expression(SExpression *expr);
void print_typed_expression(SExpression *expr);
SExpression *alloc_term(enum Term_type type);
/*void dealloc_expr(SExpression *term); 
 */
List *alloc_pair();
Lambda *alloc_lambda();/*
SExpression *duplicate_term(SExpression *term);
SExpression *duplicate_list(SExpression *head);
int list_len(SExpression *list);*/
#endif

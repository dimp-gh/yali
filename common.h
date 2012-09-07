#include "sexpr.h"
#ifndef COMMON_H
#define COMMON_H

void print_expression(SExpression *expr);
void print_typed_expression(SExpression *expr);
SExpression *alloc_term(enum Term_type type);
void dealloc_expr(SExpression *term); 
/*
SExpression *duplicate_term(SExpression *term);
SExpression *duplicate_list(SExpression *head);
int list_len(SExpression *list);*/
#endif

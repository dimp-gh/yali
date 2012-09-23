#ifndef COMMON_H
#define COMMON_H
#include "sexpr.h"


void print_expression(SExpression *expr);
void print_typed_expression(SExpression *expr);
SExpression *alloc_term(enum Term_type type);
void dealloc_expression(SExpression *term); 

SExpression *duplicate_expression(SExpression *head);
int list_length(SExpression *list);

void report_error(char *format_string, ...);

#endif

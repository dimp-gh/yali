#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "sexpr.h"
#include "hashtable.h"



typedef HashTable SymbolTable;

SExpression *eval(SExpression *expr, SymbolTable *ST);
SExpression *apply(SExpression *function, SExpression *args, SymbolTable *ST);
SExpression *handle_define(SExpression *ex, SymbolTable *ST);
SExpression *handle_if(SExpression *ex, SymbolTable *ST);
SExpression *handle_eq(SExpression *ex, SymbolTable *ST);
SExpression *handle_mul(SExpression *ex, SymbolTable *ST);
SExpression *handle_dec(SExpression *ex, SymbolTable *ST);



#endif

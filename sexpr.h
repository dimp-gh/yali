#ifndef SEXPR
#define SEXPR
#include "hashtable.h"

typedef enum { false, true } bool;

enum Term_type {
  tt_list,
  tt_int,
  tt_nil,
  tt_bool,
  tt_mention
};

typedef struct sexp {
  union {
    long int intval;
    struct sexp *exprval;
    void *nilval;
    bool boolval;
    char *mentval;
  };
  enum Term_type type;
  struct sexp *next;
} SExpression;


typedef struct {
  unsigned arity;
  SExpression *args;
  SExpression *body;
} Lambda;

typedef HashTable SymbolTable;

SExpression *eval(SExpression *expr, SymbolTable *ST);
SExpression *apply(Lambda *lambda, SExpression *args, SymbolTable *ST);
SExpression *handle_define(SExpression *ex, SymbolTable *ST);
SExpression *handle_if(SExpression *ex, SymbolTable *ST);
SExpression *handle_eq(SExpression *ex, SymbolTable *ST);
SExpression *handle_mul(SExpression *ex, SymbolTable *ST);
SExpression *handle_dec(SExpression *ex, SymbolTable *ST);

#endif

#ifndef SEXPR
#define SEXPR
#include "hashtable.h"

typedef enum { false, true } bool;

enum Term_type {
  tt_pair,
  tt_int,
  tt_nil,
  tt_bool,
  tt_lambda,
  tt_mention
};

struct List {};
struct Lambda {}; 

typedef struct sexp {
  enum Term_type type;
  union {
    List *pair;
    long int integer;
    void *nil;
    bool boolean;
    Lambda *lambda;
    char *mention;
  };
} SExpression;


typedef struct {
  unsigned arity;
  SExpression *args;
  SExpression *body;
} Lambda;

typedef struct {
  struct sexp *value;
  struct sexp *next;
} List;

typedef HashTable SymbolTable;

SExpression *eval(SExpression *expr, SymbolTable *ST);
SExpression *apply(Lambda *lambda, SExpression *args, SymbolTable *ST);
SExpression *handle_define(SExpression *ex, SymbolTable *ST);
SExpression *handle_if(SExpression *ex, SymbolTable *ST);
SExpression *handle_eq(SExpression *ex, SymbolTable *ST);
SExpression *handle_mul(SExpression *ex, SymbolTable *ST);
SExpression *handle_dec(SExpression *ex, SymbolTable *ST);

#endif

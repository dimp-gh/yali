#ifndef SEXPR
#define SEXPR

typedef enum { false, true } bool;

enum Term_type {
  tt_pair,
  tt_int,
  tt_nil,
  tt_bool,
  tt_lambda,
  tt_mention
};

struct List;
struct Lambda; 

typedef struct sexp {
  enum Term_type type;
  union {
    struct List *pair;
    long int integer;
    void *nil;
    bool boolean;
    struct Lambda *lambda;
    char *mention;
  };
} SExpression;


typedef struct Lambda {
  unsigned arity;
  SExpression *args;
  SExpression *body;
} Lambda;

typedef struct List {
  SExpression *value;
  SExpression *next;
} List;

#endif

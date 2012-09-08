#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "common.h"
#include "lexer.h"
#include "parser.h"
#include "sexpr.h"
#include "hashtable.h"
#include "evaluator.h"


void repl() {
  char buffer[256];
  SymbolTable *ST = ht_create(8);
  SExpression *test, *result;
  Token_node *tokens;
  do {
    printf(">>> ");
    gets(buffer);
    tokens = lex(buffer);
    test = parse(tokens);
    printf("Expression: "); print_expression(test);
    result = eval(test, ST);
    if (result) {
      printf("Eval = "); print_expression(result);
    } else
      printf("Evaluation error.\n");
  } while (strcmp(buffer, "(exit)") != 0);
}

int main() {
  repl();
  return 0;
}

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
  SExpression *test, *result;
  Token_node *tokens;
  init_libraries();
  do {
    printf(">>> ");
    gets(buffer);
    tokens = lex(buffer);
    test = parse(tokens);
    printf("Expression: "); print_expression(test);
    result = eval(test);
    if (result) {
      printf("Eval = "); print_expression(result);
    }
  } while (strcmp(buffer, "(exit)") != 0);
}

int main() {
  repl();
  return 0;
}

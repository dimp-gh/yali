#include "lexer.h"
#include <stdio.h>
#include <assert.h>




void lextest() {
  char teststring[] = "(define 123 #nil;  123 456 \n #t #f)";
  Token_node *head = lex(teststring);
  if (!head)
    printf("Tokenize error.\n");
  else
    printf("Tokenized.\n");
  printf("Tokens:\n");
  _print_tokens(head);
}



int main() {
  lextest();
  return 0;
}

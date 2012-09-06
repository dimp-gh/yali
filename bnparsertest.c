#include "bnparser.h"
#include "lexer.h"
#include "sexpr.h"
#include "common.h"
#include <string.h>
#include <stdio.h>


void printtest() {
  SExpression *first, *second, *third;
  SExpression *root = alloc_term(tt_pair);
  first = root->pair->value = alloc_term(tt_mention);
  first->mention = strdup("define");
  second = root->pair->next = alloc_term(tt_pair);
  second->pair->value = alloc_term(tt_mention);
  second->pair->value->mention = strdup("fifty-five");
  third = second->pair->next = alloc_term(tt_pair);
  third->pair->value = alloc_term(tt_int);
  third->pair->value->integer = 55;
  printf("What is should be: (define fifty-five 55).\n");
  printf("And what is it: ");
  print_expression(root);
}

void parsetest() {
  char str[] = "(define sqr (lambda (x) (* x x)))";
  printf("Input: %s.\n", str);
  Token_node *head = lex(str);
  printf("Tokens: ");
  _print_tokens(head);
  SExpression *root = parse(head);
  printf("After parse: ");
  print_expression(root);
}

int main() {
  //printtest();
  parsetest();
  return 0;
}

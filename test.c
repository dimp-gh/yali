#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hashtable.h"
#include "bnparser.h"
#include "lexer.h"
#include "sexpr.h"
#include "common.h"
#include "evaluator.h"


//
// HashTable tests
//

char *repr(void *data) {
  return strdup((char *) data);
}

void test_table() {
  HashTable *table = ht_create(4);
  assert(ht_lookup(table, "Doe") == NULL);
  table = ht_insert(table, "Francesko", (void *) strdup("Italy"));
  table = ht_insert(table, "Brian", (void *) strdup("America"));
  char *result;
  result = ht_lookup(table, "Francesko");
  assert(strcmp(result, "Italy") == 0);
  result = ht_lookup(table, "Mongo");
  assert(result == NULL);
  table = ht_insert(table, "Mongo", (void *) strdup("Barry"));
  result = ht_lookup(table, "Mongo");
  assert(strcmp(result, "Barry") == 0);
  table = ht_remove(table, "Mongo");
  result = ht_lookup(table, "Mongo");
  assert(result == NULL);
  //_print_table(table, repr);
  ht_delete_all(table);
  printf("All hash-table tests passed.\n");
}

// 
// Tests for common functions
//

void test_print() {
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
  print_typed_expression(root);
  dealloc_expression(root);
  printf("All print tests passed.\n");
}

//
// Lexer tests
//

void test_lex() {
  char teststring[] = "(define 123 #nil;  123 456 \n #t #f)";
  Token_node *head = lex(teststring);
  if (!head)
    printf("Tokenize error.\n");
  else
    printf("Tokenized.\n");
  printf("Tokens:\n");
  _print_tokens(head);
  printf("All lex tests passed.\n");
}

//
// Tests for new parser
//

void test_parse() {
  char str[] = "(define #t #f #nil 1 2 3 fact)";
  printf("Input: %s.\n", str);
  Token_node *head = lex(str);
  printf("Tokens: ");
  _print_tokens(head);
  SExpression *root = parse(head);
  printf("After parse: ");
  print_typed_expression(root);
  dealloc_expression(root);
  printf("All parse tests passed.\n");
}


//
// Evaluator tests
//


/*Still not present.*/

int main() {
  test_table();
  test_lex();
  test_parse();
  test_print();
  return 0;
}

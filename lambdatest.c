#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "common.h"
#include "parser.h"
#include "sexpr.h"
#include "hashtable.h"


void testsexp() {
  SExpression *root, *first, *second, *third, *third1, *third2, *third3, *fourth;
  root = alloc_term(tt_list);
  root->exprval = first = alloc_term(tt_int);
  first->intval = 4;
  first-> next = second = alloc_term(tt_int);
  second->intval = 8;
  second->next = third = alloc_term(tt_list);
  third->exprval = third1 = alloc_term(tt_bool);
  third1->boolval = true;
  third1->next = third2 = alloc_term(tt_mention);
  third2->mentval = strdup("funcall");
  third2->next = third3 = alloc_term(tt_nil);
  third->next = fourth = alloc_term(tt_int);
  fourth->intval = 142857;
  print_expr(root);
  dealloc_expr(root);
}

void testparse() {
  char buffer[] = "(define fact (x) (if (eq x 1) 1 (* x (fact (dec x)))))";
  printf("Start text:\t\t%s.\n", buffer);
  SExpression *ex = parse(&buffer[0]);
  printf("Parsed representation:\t");
  print_expr(ex);
  //  printf("Parsed representation with types: ");
  //  print_typed_expr(ex);
  dealloc_expr(ex);
}

void repl() {
  char buffer[256];
  SymbolTable *ST = ht_create();
  SExpression *test, *result;
  do {
    printf(">>> ");
    gets(buffer);
    test = parse(&buffer[0]);
    printf("Expression: "); print_expr(test);
    result = eval(test, ST);
    if (result) {
      printf("Eval = "); print_expr(result);
    } else
      printf("Evaluation error.\n");
  } while (strcmp(buffer, "(exit)") != 0);
}

int main() {
  //testsexp();
  //testparse();
  repl();
  return 0;
}

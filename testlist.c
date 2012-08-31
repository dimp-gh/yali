#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "linklist.h"



/*void test_list() {
  ll_node *list = NULL;
  int value, i;
  assert(ll_lookup(list, 880) == 0);
  for (i = 0; i < 5; i++) {
    list = ll_insert(list, i);
  }
  _print_list(list);
  assert(ll_lookup(list, 2) != 0); 
  assert(ll_lookup(list, 18) == 0);
  list = ll_insert(list, 18);
  assert(ll_lookup(list, 18) != 0);
  list = ll_delete(list, 18);
  assert(ll_lookup(list, 18) == 0);
  ll_clear_list(list);
  printf("Tests passed.\n");
}*/

void testnodes() {
  ll_node *head = NULL;
  char *buf1, *buf2;
  printf("Create.\n");
  buf1 = strdup("Franchesko");
  head = ll_insert(head, buf1, (void *) buf1); 
  buf1 = strdup("Brian");
  head = ll_insert(head, buf1, (void *) buf1);
  printf("Print.\n");
  _print_list(head);
  printf("Remove.\n");
  head = ll_remove(head, "Brian");
  printf("Print.\n");
  _print_list(head);
  ll_clear_list(head);
}




int main() {	  
  testnodes();
  return 0;
}

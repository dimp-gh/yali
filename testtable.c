#include <stdio.h>
#include <string.h>
#include "hashtable.h"
#include <assert.h>

char *repr(void *data) {
  return strdup((char *) data);
}

void testtable() {
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


int main() {
  testtable();
  return 0;
}

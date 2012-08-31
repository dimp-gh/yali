#include <malloc.h>
#include <stdio.h>
#include "hashtable.h"

//#define DEBUG(X) printf("Debug: %s.\n", X)
#define DEBUG(X)  

int _hash_function(char *key) {
  char *curr = key;
  int sum = 0;
  if (!key)
    return -1;
  while (*(curr++) != '\0')
    sum += *curr;
  return sum % BAG_COUNT;
}


HashTable *ht_create() {
  HashTable *table = (HashTable *) malloc(sizeof(HashTable));
  table->hash = _hash_function;
  int i;
  for (i = 0; i < BAG_COUNT; i++)
    table->bags[i] = NULL;
  return table;
}


HashTable *ht_insert(HashTable *table, char *key, void *data) {
  DEBUG("Inserting element in hashtable");
  int bag = table->hash(key);
  ll_node *head = table->bags[bag];
  head = ll_insert(head, key, data);
  table->bags[bag] = head;
  return table;
}


void *ht_lookup(HashTable *table, char *key) {
  int hash = table->hash(key);
  if (hash == -1)
    return NULL;
  else
    return ll_lookup(table->bags[hash], key);
}


HashTable *ht_remove(HashTable *table, char *key) {
  int hash = table->hash(key);
  if (hash == -1)
    return table;
  table->bags[hash] = ll_remove(table->bags[hash], key);
  return table;
}


void ht_delete_all(HashTable *table) {
  int i;
  for (i = 0; i < BAG_COUNT; i++)
    ll_clear_list(table->bags[i]);
  free(table);
}

void _print_table(HashTable *table) {
  int i;
  for (i = 0; i < BAG_COUNT; i++) {
    printf("Bag #%d:\n", i); 
    _print_list(table->bags[i]);
  }
}


#ifndef HASH_TABLE
#define HASH_TABLE
#include "linklist.h"

#define BAG_COUNT 16


typedef struct {
  ll_node *bags[BAG_COUNT];
  int (*hash)(char *);
} HashTable; 


int _hash_function(char *key);
HashTable *ht_create();
HashTable *ht_insert(HashTable *table, char *key, void *data);
void *ht_lookup(HashTable *table, char *key);
HashTable *ht_remove(HashTable *table, char *key);
void ht_delete_all(HashTable *table);
void _print_table(HashTable *table);


#endif

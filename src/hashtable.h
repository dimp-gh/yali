#ifndef HASH_TABLE
#define HASH_TABLE

#define DEF_BAG_COUNT 16


typedef struct node_el {
  char *key;
  void *data;
  struct node_el *next;
} bag_node;
// Hashtable's bag node.

typedef struct {
  bag_node **bags;
  int bag_count;
  int (*hash)(char *, int);
} HashTable;
// HashTable is array of bags, every bag is linked list.

int _hash_function(char *key, int bag_count);
// Example hash function.
// Used with new hashtable by default.
// If key is empty, yields -1.
// Otherwise returns hash_value from [0; bag_count).

HashTable *ht_create(int _bag_count);
// Create new hashtable with _bag_count (>= 2) bags.
// Returns pointer to created hashtable.

HashTable *ht_insert(HashTable *table, char *key, void *data);
// Insert data with key into non-empty hashtable.
// If table is NULL — returns NULL.
// If key is NULL or data is NULL — returns pointer to hashtable.
// Data must be allocated, key will be duplicated for storing.
// Returns pointer to HashTable.

void *ht_lookup(HashTable *table, char *key);
// Lookup for data with given key in table.
// If table is NULL or key is NULL — returns NULL.
// If there's no such data with given key — returns NULL.
// If data with given key is found — returns pointer to data.

HashTable *ht_remove(HashTable *table, char *key);
// Remove data with given key from hashtable.
// If table is NULL — returns NULL.
// If key is NULL — returns table.
// Removes data and key (if there's such data with given key)
// and returns pointer to table.

void ht_delete_all(HashTable *table);
// Delete table and all stored values.
// If table is NULL — does nothing.

void _print_table(HashTable *table, char *(*repr)(void *));
// Prints table representation in format:
// Bag 0:
//  %value1%
//  %value2%
//  ...
// Bag 1:
//  %value3%
//  %value4%
//  ...
// ...
// Values are made by function repr, which takes
// pointer to void data and returns (char *) representation of data.

#endif

#ifndef HASH_TABLE
#define HASH_TABLE

#define DEF_BAG_COUNT 16

typedef struct node_el {
  char *key;
  void *data;
  struct node_el *next;
} bag_node;

typedef struct {
  bag_node **bags;
  int bag_count;
  int (*hash)(char *, int);
} HashTable; 


bag_node *bag_insert(bag_node *head, char *key, void *value);
bag_node *bag_remove(bag_node *head, char *key);
void *bag_lookup(bag_node *head, char *key);
void bag_clear_list(bag_node *head);
void _print_list(bag_node *head);

int _hash_function(char *key, int bag_count);
HashTable *ht_create(int _bag_count);
HashTable *ht_insert(HashTable *table, char *key, void *data);
void *ht_lookup(HashTable *table, char *key);
HashTable *ht_remove(HashTable *table, char *key);
void ht_delete_all(HashTable *table);
void _print_table(HashTable *table);


#endif

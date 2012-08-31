#ifndef LINKLIST
#define LINKLIST

typedef struct node_el {
  char *key;
  void *data;
  struct node_el *next;
} ll_node;


ll_node *ll_insert(ll_node *head, char *key, void *value);
ll_node *ll_remove(ll_node *head, char *key);
void *ll_lookup(ll_node *head, char *key);
void ll_clear_list(ll_node *head);
void _print_list(ll_node *head);


#endif

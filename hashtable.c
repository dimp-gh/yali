#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

//#define DEBUG(X) printf("Debug: %s.\n", X)
#define DEBUG(X)  

bag_node *bag_insert(bag_node *head, char *key, void *value) {
  DEBUG("Inserting new element in list");
  bag_node *new = (bag_node *) malloc(sizeof(bag_node));
  new->key = strdup(key);
  new->data = value;
  new->next = NULL;
  if (!head)
    return new;
  else {
    bag_node *current = head;
    while (current->next)
      current = current->next;
    current->next = new;
    return head;
  }
}


bag_node *bag_remove(bag_node *head, char *key) {
  DEBUG("Removing element from list");
  bag_node *current = head;
  if (!current)
    return head;
  else {
    char found = 0;
    bag_node *previous = NULL;
    while (current && !found) {
      if (strcmp(current->key, key) == 0) {
	found = 1;
	break;
      }
      previous = current;
      current = current->next;
    }
    if (found) {
      if (previous) {
	bag_node *tail = current->next;
	previous->next = tail;
	free(current->key);
	free(current->data);
	free(current);
	return head;
      } else {
	bag_node *tail = current->next;
	free(current->key);
	free(current->data);
	free(current);
	return tail;
      }
    }
    return head;
  }
}

void *bag_lookup(bag_node *head, char *key) {
  DEBUG("Searching in list");
  bag_node *current = head;
  while (current) {
    if (strcmp(current->key, key) == 0)
      return current->data;
    current = current->next;
  }
  return NULL;
}


void bag_clear_list(bag_node *head) {
  DEBUG("Clearing list");
  if (!head)
    return;
  bag_node *current = head;
  bag_node *next;
  while (current) {
    next = current->next;
    free(current->key);
    free(current->data);
    free(current);
    current = next;
  }
}


void _print_list(bag_node *head) {
  bag_node *current = head;
  if (!current)
    printf("<Empty list>.\n");
  else {
    while (current) {
      printf("%s - %p.\n", current->key, current->data);
      current = current->next;
    }
  }
}


int _hash_function(char *key, int bag_count) {
  char *curr = key;
  int sum = 0;
  if (!key)
    return -1;
  while (*(curr++) != '\0')
    sum += *curr;
  return sum % bag_count;
}


HashTable *ht_create(int _bag_count) {
  HashTable *table = malloc(sizeof(HashTable));
  table->bag_count = (_bag_count >= 2) ? _bag_count : DEF_BAG_COUNT;
  table->bags = malloc(table->bag_count * sizeof(bag_node *));
  table->hash = _hash_function;
  int i;
  for (i = 0; i < table->bag_count; i++)
    table->bags[i] = NULL;
  return table;
}


HashTable *ht_insert(HashTable *table, char *key, void *data) {
  DEBUG("Inserting element in hashtable");
  int bag = table->hash(key, table->bag_count);
  bag_node *head = table->bags[bag];
  head = bag_insert(head, key, data);
  table->bags[bag] = head;
  return table;
}


void *ht_lookup(HashTable *table, char *key) {
  int hash = table->hash(key, table->bag_count);
  if (hash == -1)
    return NULL;
  else
    return bag_lookup(table->bags[hash], key);
}


HashTable *ht_remove(HashTable *table, char *key) {
  int hash = table->hash(key, table->bag_count);
  if (hash == -1)
    return table;
  table->bags[hash] = bag_remove(table->bags[hash], key);
  return table;
}


void ht_delete_all(HashTable *table) {
  int i;
  for (i = 0; i < table->bag_count; i++)
    bag_clear_list(table->bags[i]);
  free(table);
}

void _print_table(HashTable *table) {
  int i;
  for (i = 0; i < table->bag_count; i++) {
    printf("Bag #%d:\n", i); 
    _print_list(table->bags[i]);
  }
}


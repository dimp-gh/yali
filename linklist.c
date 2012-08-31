#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "linklist.h"


//#define DEBUG(X) printf("Debug: %s.\n", X)
#define DEBUG(X)  

ll_node *ll_insert(ll_node *head, char *key, void *value) {
  DEBUG("Inserting new element in list");
  ll_node *new = (ll_node *) malloc(sizeof(ll_node));
  new->key = strdup(key);
  new->data = value;
  new->next = NULL;
  if (!head)
    return new;
  else {
    ll_node *current = head;
    while (current->next)
      current = current->next;
    current->next = new;
    return head;
  }
}


ll_node *ll_remove(ll_node *head, char *key) {
  DEBUG("Removing element from list");
  ll_node *current = head;
  if (!current)
    return head;
  else {
    char found = 0;
    ll_node *previous = NULL;
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
	ll_node *tail = current->next;
	previous->next = tail;
	free(current->key);
	free(current->data);
	free(current);
	return head;
      } else {
	ll_node *tail = current->next;
	free(current->key);
	free(current->data);
	free(current);
	return tail;
      }
    }
    return head;
  }
}

void *ll_lookup(ll_node *head, char *key) {
  DEBUG("Searching in list");
  ll_node *current = head;
  while (current) {
    if (strcmp(current->key, key) == 0)
      return current->data;
    current = current->next;
  }
  return NULL;
}


void ll_clear_list(ll_node *head) {
  DEBUG("Clearing list");
  if (!head)
    return;
  ll_node *current = head;
  ll_node *next;
  while (current) {
    next = current->next;
    free(current->key);
    free(current->data);
    free(current);
    current = next;
  }
}


void _print_list(ll_node *head) {
  ll_node *current = head;
  if (!current)
    printf("<Empty list>.\n");
  else {
    while (current) {
      printf("%s - %p.\n", current->key, current->data);
      current = current->next;
    }
  }
}

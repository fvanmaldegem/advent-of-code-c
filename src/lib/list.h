#pragma once

#include <stdio.h>

typedef struct Node {
    void *data;
    struct Node *next;
    struct Node *prev;
} t_node;

typedef struct List {
    const size_t data_size;
    size_t len;
    t_node *head;
    t_node *tail;
} t_list;

// memory related list functions

t_list *list_allocate(size_t data_size);
void list_free(t_list **list, void (*data_free)(void *));
void node_free(t_node *node, void (*data_free)(void *)) ;


// insertions related list functions 

void list_append(t_list *list, void *data);
void list_push(t_list *list, void *data);


// getting nodes from a list/removing them

int list_get_index(t_list *list, void *data);
t_node *list_get_by_index(t_list* list, size_t i);

// removes a node by it's index and returns the data pointer inside. You will also need to clean up this data. 
void *list_remove_by_index(t_list* list, size_t i);
void *list_pop(t_list *list);

// clone a list
t_list *list_clone(t_list *src, bool with_nodes);


// print a list
void list_fprint(FILE *restrict stream, t_list *list, void (*printfunc)(FILE *restrict stream, void *));
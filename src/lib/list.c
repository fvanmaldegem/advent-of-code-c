#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

t_node *__node_allocate(void *data) {
    const t_node node = {.data = data};

    t_node *allocated_node = malloc(sizeof(node));
    void *ret = memcpy(allocated_node, &node, sizeof(node));
    assert(ret);

    return allocated_node;
}

void node_free(t_node *node, void (*data_free)(void *)) {
    if (data_free) {
        data_free(node->data);
        node->data = NULL;
    }

    free(node);
}

t_list *list_allocate(size_t data_size) {
    const t_list list = {.data_size = data_size};

    t_list *allocated_list = malloc(sizeof(list));
    void *ret = memcpy(allocated_list, &list, sizeof(list));
    assert(ret);

    return allocated_list;
}

void list_append(t_list *list, void *data) {
    t_node *new_node = __node_allocate(data);

    if (list->head == NULL && list->tail == NULL) {
        list->head = new_node;
    } else {
        t_node *cur_node = list->head;

        while (cur_node->next != NULL) {
            cur_node = cur_node->next;
        }

        new_node->prev = cur_node;
        cur_node->next = new_node;
    }

    list->tail = new_node;
    list->len++;
}

void list_push(t_list *list, void *data) {
    t_node *new_node = __node_allocate(data);

    if (list->head == NULL && list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->head->prev = new_node;
        new_node->next = list->head;
    }

    list->head = new_node;
    list->len++;
}

t_node *list_get_by_index(t_list *list, size_t i) {
    if (list->len == 0 || i >= list->len) {
        return NULL;
    }

    t_node *cur_node = list->head;

    for (int j = 0; j < list->len; j++) {
        if (i == j) {
            return cur_node;
        }

        cur_node = cur_node->next;
    }

    return NULL;
}

int list_get_index(t_list *list, void *data) {
    if (list->len == 0 || data == NULL) {
        return -1;
    }

    for (int i = 0; i < list->len; i++) {
        if (list_get_by_index(list, i)->data == data) {
            return i;
        }
    }

    return -1;
}

void *list_remove_by_index(t_list *list, size_t i) {
    void *value = NULL;

    if (list->len == 0 || i >= list->len) {
        return value;
    }

    t_node *cur_node = list->head;

    for (int j = 0; j < list->len; j++) {
        if (i == j) {
            value = cur_node->data;
            if (cur_node == list->head) {
                list->head = cur_node->next;
            }

            if (cur_node == list->tail) {
                list->tail = cur_node->prev;
            }

            if (cur_node->prev != NULL) {
                cur_node->prev->next = cur_node->next;
            }

            if (cur_node->next != NULL) {
                cur_node->next->prev = cur_node->prev;
            }
            
            cur_node->next = NULL;
            cur_node->prev = NULL;

            list->len--;
            node_free(cur_node, 0);
            break;
        }

        cur_node = cur_node->next;
    }

    return value;
}

void *list_pop(t_list *list) {
    return list_remove_by_index(list, 0);
}

t_list *list_clone(t_list *src, bool with_nodes) {
    t_list *target = list_allocate(src->data_size); 

    if (!with_nodes) {
        return memcpy(target, src, sizeof(t_list));
    }

    for (int i = 0; i < src->len; i++) {
        t_node *old_node = list_get_by_index(src, i);
        void *pdata = malloc(sizeof(src->data_size));
        memcpy(pdata, old_node->data, src->data_size);
        list_append(target, pdata);
    }

    return target;
}

void list_free(t_list **list, void (*data_free)(void *)) {
    t_node *node = (*list)->head;

    for (size_t i = 0; i < (*list)->len; i++) {
        t_node *next = node->next;
        node_free(node, data_free);
        node = next;
    }

    free(*list);
    *list = NULL;
}

void list_fprint(FILE *restrict stream, t_list *list, void (*printfunc)(FILE *restrict stream, void *)) {
    fprintf(stream, "[");
    t_node *node = list->head; 
    while (node != NULL) {
        printfunc(stream, node->data);

        if (node->next != NULL) {
            fprintf(stream, ", ");
        }

        node = node->next;
    }

    fprintf(stream, "]\n");
}
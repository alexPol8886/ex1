//
// Created by student on 12/5/24.
//
#include <stdio.h>
#include "linked_list.h"

int add(LinkedList *link_list, void *data) {
    if (!link_list) {
        fprintf(stderr, "Error: Linked list is not initialized.\n");
        return 1;
    }

    Node *new_node = malloc(sizeof(Node));
    if (!new_node) {
        return 1;
    }

    new_node->data = data;
    new_node->next = NULL;

    if (link_list->first == NULL) {
        link_list->first = new_node;
        link_list->last = new_node;
    } else {
        link_list->last->next = new_node;
        link_list->last = new_node;
    }

    link_list->size++;
    return 0;
}

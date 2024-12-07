//
// Created by student on 12/5/24.
//

#ifndef EX1_LINKED_LIST_H
#define EX1_LINKED_LIST_H

#include <stdlib.h> // For malloc()

typedef struct Node {
    struct MarkovNode *data;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *first;
    Node *last;
    int size;
} LinkedList;

/**
 * Add data to new markov_node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add (LinkedList *link_list, void *data);

#endif //EX1_LINKED_LIST_H

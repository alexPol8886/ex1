//
// Created by student on 12/5/24.
//

#ifndef EX1_MARKOV_CHAIN_H
#define EX1_MARKOV_CHAIN_H


#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ALLOCATION_ERROR_MESSAGE "Allocation failure: Failed to allocate new memory\n"

// Structure to store a word node in the Markov chain
typedef struct MarkovNode {
    char *data; // The word itself
    struct MarkovNodeFrequency *frequency_list; // List of possible next words and their frequencies
    int frequency_list_size;
} MarkovNode;

// Structure to store frequency of a word appearing after another word
typedef struct MarkovNodeFrequency {
    struct MarkovNode *markov_node; // Next possible word
    int frequency; // Frequency of this word following the current word
} MarkovNodeFrequency;

// Structure for the Markov chain, which contains all nodes
typedef struct MarkovChain {
    LinkedList *database; // Linked list of all nodes in the Markov chain
} MarkovChain;

// Function prototypes
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr);
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr);
int add_node_to_frequency_list(MarkovNode *first_node, MarkovNode *second_node);
MarkovNode* get_first_random_node(MarkovChain *markov_chain);
MarkovNode* get_next_random_node(MarkovNode *cur_markov_node);
void generate_tweet(MarkovChain *markov_chain, int max_length);
void free_database(LinkedList **ptr_chain);



#endif //EX1_MARKOV_CHAIN_H

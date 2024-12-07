//
// Created by student on 12/5/24.
//
#include "markov_chain.h"
#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


char *my_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy) {
        memcpy(copy, s, len);
    }
    return copy;
}

// Function to get a random number between 0 and max_number [0, max_number)
int get_random_number(int max_number) {
    return rand() % max_number;
}

// Function to add a word to the database, or return the existing node
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr) {
    if (!markov_chain || !data_ptr) return NULL;

    // Ensure the linked list is initialized
    if (!markov_chain->database) {
        fprintf(stderr, "Error: Markov chain database is not initialized.\n");
        return NULL;
    }

    // Check if the word already exists in the database
    Node *node = get_node_from_database(markov_chain, data_ptr);
    if (node) return node; // Word already in database

    // Word doesn't exist, so create a new node
    MarkovNode *new_node = malloc(sizeof(MarkovNode));
    if (!new_node) {
        fprintf(stderr, ALLOCATION_ERROR_MESSAGE);
        return NULL;
    }

    new_node->data = my_strdup(data_ptr); // Duplicate the word string
    if (!new_node->data) {
        free(new_node);
        return NULL;
    }
    new_node->frequency_list = NULL;

    // Add the new node to the linked list
    if (add(markov_chain->database, new_node) != 0) {
        free(new_node->data);
        free(new_node);
        return NULL;
    }

    return (Node *)new_node;
}



// Function to update the frequency list of a node
int add_node_to_frequency_list(MarkovNode *first_node, MarkovNode *second_node) {
    if (!first_node || !second_node) return 1; // Error if any node is NULL

    // Check if second_node is already in the frequency list
    for (int i = 0; i < first_node->frequency_list_size; i++) {
        if (first_node->frequency_list[i].markov_node == second_node) {
            first_node->frequency_list[i].frequency++;
            return 0;
        }
    }

    // Add second_node to the frequency list
    MarkovNodeFrequency *new_list = realloc(first_node->frequency_list,
                                            (first_node->frequency_list_size + 1) * sizeof(MarkovNodeFrequency));
    if (!new_list) {
        fprintf(stderr, ALLOCATION_ERROR_MESSAGE);
        return 1;
    }

    first_node->frequency_list = new_list;
    first_node->frequency_list[first_node->frequency_list_size].markov_node = second_node;
    first_node->frequency_list[first_node->frequency_list_size].frequency = 1;
    first_node->frequency_list_size++;

    return 0;
}


// Function to get a random node from the Markov chain
MarkovNode* get_first_random_node(MarkovChain *markov_chain) {
    if (!markov_chain || markov_chain->database->size == 0) return NULL;
    int random_index = get_random_number(markov_chain->database->size);
    Node *current = markov_chain->database->first;
    for (int i = 0; i < random_index; i++) {
        current = current->next;
    }
    return (MarkovNode *)current->data;
}

// Function to choose the next random node based on frequency
MarkovNode* get_next_random_node(MarkovNode *cur_markov_node) {
    if (!cur_markov_node || cur_markov_node->frequency_list_size == 0) return NULL;

    // Get the total frequency
    int total_frequency = 0;
    for (int i = 0; i < cur_markov_node->frequency_list_size; i++) {
        total_frequency += cur_markov_node->frequency_list[i].frequency;
    }

    // Select a random number between 0 and total_frequency
    int random_number = get_random_number(total_frequency);
    int cumulative_frequency = 0;

    // Find the corresponding node based on the random number
    for (int i = 0; i < cur_markov_node->frequency_list_size; i++) {
        cumulative_frequency += cur_markov_node->frequency_list[i].frequency;
        if (random_number < cumulative_frequency) {
            return cur_markov_node->frequency_list[i].markov_node;
        }
    }

    return NULL; // Should never reach here if the frequency list is valid
}

// Function to generate and print a random tweet
void generate_tweet(MarkovChain *markov_chain, int max_length) {
    if (!markov_chain || max_length < 2) return;

    // Get the first random word to start the tweet
    MarkovNode *current_node = get_first_random_node(markov_chain);
    if (!current_node) return;

    printf("%s", current_node->data); // Print the first word

    // Generate the tweet by selecting words based on frequency
    for (int i = 1; i < max_length; i++) {
        current_node = get_next_random_node(current_node);
        if (!current_node) break;
        printf(" %s", current_node->data);
    }

    printf("\n");
}

// Function to get a node from the database
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr) {
    if (!markov_chain || !data_ptr) return NULL;
    if (!markov_chain->database || !markov_chain->database->first) {
        fprintf(stderr, "Error: Database is empty or not initialized.\n");
        return NULL;
    }
    Node *current = markov_chain->database->first;
    while (current) {
        if (current->data) {
            MarkovNode *node = (MarkovNode *)current->data;
            if (strcmp(node->data, data_ptr) == 0) {
                return current;
            }
        }
        current = current->next;
    }
    return NULL;
}


void free_database(LinkedList **ptr_chain) {
    if (!ptr_chain || !*ptr_chain) return;

    Node *current = (*ptr_chain)->first;
    while (current) {
        Node *next = current->next;
        MarkovNode *node = (MarkovNode *)current->data;
        free(node->data); // Free the word string
        free(node->frequency_list); // Free the frequency list
        free(node); // Free the MarkovNode
        free(current); // Free the Node
        current = next;
    }
    free(*ptr_chain); // Free the LinkedList itself
    *ptr_chain = NULL; // Set the pointer to NULL
}





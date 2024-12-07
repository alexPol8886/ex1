//
// Created by student on 12/5/24.
//
#define FILE_PATH_ERROR "Error: incorrect file path"
#define NUM_ARGS_ERROR "Usage: invalid number of arguments"
#define DELIMITERS " \n\t\r"
#include "markov_chain.h"
#include <string.h>


// Function to process the tweet file and build the Markov chain
void process_tweet_file(const char *file_path, MarkovChain *markov_chain) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, FILE_PATH_ERROR);
        return;
    }

    char word[256];
    char prev_word[256] = ""; // To hold the previous word in the tweet

    // Read words from the file
    while (fscanf(file, "%255s", word) == 1) {
        // Skip any delimiters like punctuation or hashtags
        if (word[0] == '#' || word[0] == '@' || word[0] == '.') continue;

        // Add the current word to the database
        MarkovNode *current_node = (MarkovNode *)add_to_database(markov_chain, word);

        if (strlen(prev_word) > 0) {
            // If there's a previous word, get its node and update the frequency list
            MarkovNode *prev_node = (MarkovNode *)get_node_from_database(markov_chain, prev_word);
            add_node_to_frequency_list(prev_node, current_node); // Update frequency
        }

        // Set the current word as previous for the next iteration
        strncpy(prev_word, word, sizeof(prev_word) - 1);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    // Check for the correct number of arguments
    if (argc != 4) {
        fprintf(stderr, NUM_ARGS_ERROR);
        return 1;
    }

    // Parse command-line arguments
    unsigned int seed = atoi(argv[1]);       // Seed for the random number generator
    int num_tweets = atoi(argv[2]);         // Number of tweets to generate
    const char *file_path = argv[3];        // Path to the input file

    // Validate arguments
    if (num_tweets <= 0) {
        fprintf(stderr, "Error: Number of tweets must be a positive integer.\n");
        return 1;
    }

    // Initialize the random number generator
    srand(seed);

    // Initialize the Markov chain
    MarkovChain markov_chain;
    markov_chain.database = malloc(sizeof(LinkedList));
    if (!markov_chain.database) {
        fprintf(stderr, ALLOCATION_ERROR_MESSAGE);
        return 1;
    }
    markov_chain.database->first = NULL;
    markov_chain.database->last = NULL;
    markov_chain.database->size = 0;

    // Open the corpus file
    FILE *file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, FILE_PATH_ERROR);
        free(markov_chain.database);
        return 1;
    }

    // Process the file to build the Markov chain
    char word[256];
    char prev_word[256] = ""; // To track the previous word
    while (fscanf(file, "%255s", word) == 1) {
        MarkovNode *current_node = (MarkovNode *)add_to_database(&markov_chain, word);
        if (strlen(prev_word) > 0) {
            MarkovNode *prev_node = (MarkovNode *)get_node_from_database(&markov_chain, prev_word);
            add_node_to_frequency_list(prev_node, current_node);
        }
        strncpy(prev_word, word, sizeof(prev_word) - 1);
    }
    fclose(file);

    // Generate and print the requested number of tweets
    for (int i = 0; i < num_tweets; i++) {
        printf("Tweet %d: ", i + 1);
        generate_tweet(&markov_chain, 20); // Assuming max_length = 20
    }

    // Free the Markov chain resources
    free_database(&markov_chain.database);

    return 0;
}


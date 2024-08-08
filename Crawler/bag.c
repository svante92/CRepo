#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "set.h"
#include "crawler.h"
#include "bag.h"
#include "webpage.h"

/*
 * bag.c - Implimentation of the bag ADT that contains webpages for the crawler to crawl.
 *
 * Crawler, Svante Aretun, svante92
 */

#define INITIAL_CAPACITY 10

bag_t* create_bag() {
    bag_t *bag = (bag_t *)malloc(sizeof(bag_t));
    if (bag == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    bag->webpages = (webpage_t **)malloc(INITIAL_CAPACITY * sizeof(webpage_t **));
    if (bag->webpages == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    bag->capacity = INITIAL_CAPACITY;
    bag->size = 0;

    return bag;
}

void destroy_bag(bag_t *bag) {
    for (size_t i = 0; i < bag->size; ++i) {
		webpage_delete(bag->webpages[i]);
    }
    free(bag->webpages);
    free(bag);
}

void add_to_bag(bag_t *bag, webpage_t *webpage) {
    if (bag->size == bag->capacity) {
        bag->capacity <<= 1;
        bag->webpages = realloc(bag->webpages, bag->capacity * sizeof(webpage_t *));
        if (bag->webpages == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
    }

    // Add the new webpage to the bag
    bag->webpages[bag->size] = webpage;
    bag->size++;
}

int is_empty(const bag_t *bag) {
    return bag->size == 0;
}

webpage_t *remove_from_bag(bag_t *bag) {
    if (is_empty(bag)) {
        fprintf(stderr, "Error: bag_t is empty\n");
        exit(EXIT_FAILURE);
    }

    // Remove the first webpage from the bag
    webpage_t *removed = bag->webpages[0];

    // Shift remaining webpages to fill the gap
    for (size_t i = 1; i < bag->size; ++i) {
        bag->webpages[i - 1] = bag->webpages[i];
    }

    bag->size--;

    return removed;
}

int contains_url(const bag_t *bag, const char *url, size_t *depth) {
    for (size_t i = 0; i < bag->size; ++i) {
        if (strcmp(bag->webpages[i]->url, url) == 0) {
            *depth = bag->webpages[i]->depth;
            return 1;  // URL found in the bag
        }
    }

    return 0;  // URL not found in the bag
}

/*
 * hashtable.c - Implimentation of the hashtable ADT that maintains an unordered collection of (key, item) pairs just like a set except it uses a hashing function to create the keys.
 *
 * Crawler, Svante Aretun, svante92
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h> /* For uintptr_t */

#include "set.h"
#include "crawler.h"
#include "hashtable.h"

// Structure representing a hashtable
struct hashtable {
    int num_slots;
    set_t **slots;
};

// Function to create a new hashtable
hashtable_t *hashtable_new(const int num_slots) {
    if (num_slots <= 0) {
        return NULL;  // Invalid number of slots
    }

    hashtable_t *new_ht = malloc(sizeof(hashtable_t));
    if (new_ht == NULL) {
        return NULL;  // Memory allocation failed
    }

    new_ht->num_slots = num_slots;
    
    // Allocate memory for array of sets
    new_ht->slots = malloc(num_slots * sizeof(set_t *));
    if (new_ht->slots == NULL) {
        free(new_ht);
        return NULL;  // Memory allocation failed
    }

    // Initialize each slot with an empty set
    for (int i = 0; i < num_slots; i++) {
        new_ht->slots[i] = set_new();
        if (new_ht->slots[i] == NULL) {
            // Clean up and return NULL if set creation fails
            for (int j = 0; j < i; j++) {
                set_delete(new_ht->slots[j], NULL);
            }
            free(new_ht->slots);
            free(new_ht);
            return NULL;
        }
    }

    return new_ht;
}

// Hashing function treating the key as an integer and using modulus
static int hash_function(const char *key, int num_slots) {
    int hash = 0;
    for (const char *c = key; *c != '\0'; c++) {
        hash = (hash * 31 + *c) % num_slots;
    }
    return hash;
}

// Function to insert an item into the hashtable
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL) {
        return false;  // Invalid parameters
    }

    int slot_index = hash_function(key, ht->num_slots);

    // Insert item into the corresponding set in the selected slot
    return set_insert(ht->slots[slot_index], key, item);
}

// Function to find an item in the hashtable
void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht == NULL || key == NULL) {
        return NULL;  // Invalid parameters
    }

    int slot_index = hash_function(key, ht->num_slots);

    // Find item in the corresponding set in the selected slot
    return set_find(ht->slots[slot_index], key);
}

// Function to print the hashtable
void hashtable_print(hashtable_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL) {
        return;  // Invalid file pointer
    }

    if (ht != NULL) {
        for (int i = 0; i < ht->num_slots; i++) {
            set_print(ht->slots[i], fp, itemprint);
        }
    }
}

// Function to iterate over all items in the hashtable
void hashtable_iterate(hashtable_t *ht, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (ht == NULL || itemfunc == NULL) {
        return;  // Invalid parameters
    }

    for (int i = 0; i < ht->num_slots; i++) {
        set_iterate(ht->slots[i], arg, itemfunc);
    }
}

// Function to delete the whole hashtable
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht == NULL) {
        return;  // Nothing to delete
    }

    for (int i = 0; i < ht->num_slots; i++) {
        set_delete(ht->slots[i], itemdelete);
    }

    free(ht->slots);
    free(ht);
}

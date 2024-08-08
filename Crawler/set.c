/*
 * set.c - Implimentation of the set ADT that maintains an unordered collection of (key, item) pairs; any given key can only occur in the set once
 *
 * Crawler, Svante Aretun, svante92
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"

// Structure representing a node in the set
struct setnode {
    char *key;
    void *item;
    struct setnode *next;
};

// Structure representing the set
struct set {
    struct setnode *head;
};

// Function to create a new set
set_t *set_new(void) {
    set_t *new_set = malloc(sizeof(set_t));
    if (new_set == NULL) {
        return NULL;  // Memory allocation failed
    }

    new_set->head = NULL;
    return new_set;
}

// Function to insert an item into the set
bool set_insert(set_t *set, const char *key, void *item) {
    if (set == NULL || key == NULL) {
        return false;  // Invalid parameters
    }

    // Check if the key already exists
    struct setnode *current = set->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return false;  // Key already exists
        }
        current = current->next;
    }

    // Create a new node
    struct setnode *new_node = malloc(sizeof(struct setnode));
    if (new_node == NULL) {
        return false;  // Memory allocation failed
    }

    // Copy the key
    new_node->key = strdup(key);
    if (new_node->key == NULL) {
        free(new_node);
        return false;  // Memory allocation failed
    }

    new_node->item = item;
    new_node->next = set->head;
    set->head = new_node;
    return true;  // Item successfully inserted
}

// Function to find an item in the set
void *set_find(set_t *set, const char *key) {
    if (set == NULL || key == NULL) {
        return NULL;  // Invalid parameters
    }

    struct setnode *current = set->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->item;  // Key found, return the associated item
        }
        current = current->next;
    }

    return NULL;  // Key not found
}

// Function to print the set
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL) {
        return;  // Invalid file pointer
    }

    if (set != NULL) {
        struct setnode *current = set->head;
        while (current != NULL) {
            if (itemprint != NULL) {
                itemprint(fp, current->key, current->item);
            } else {
                fprintf(fp, "(null)");
            }

            current = current->next;
        }
    }
}

// Function to iterate over all items in the set
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (set == NULL || itemfunc == NULL) {
        return;  // Invalid parameters
    }

    struct setnode *current = set->head;
    while (current != NULL) {
        itemfunc(arg, current->key, current->item);
        current = current->next;
    }
}

// Function to delete the whole set
void set_delete(set_t *set, void (*itemdelete)(void *item)) {
    if (set == NULL) {
        return;  // Nothing to delete
    }

    struct setnode *current = set->head;
    while (current != NULL) {
        struct setnode *next = current->next;

        // Delete the key
        free(current->key);

        // Delete the item if a delete function is provided
        if (itemdelete != NULL) {
            itemdelete(current->item);
        }

        // Delete the node
        free(current);

        current = next;
    }

    // Delete the set structure
    free(set);
}

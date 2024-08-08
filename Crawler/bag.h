#ifndef BAG_H
#define BAG_H

#include <stddef.h>

// Creates a new bag
bag_t* create_bag();

// Destroys the given bag, freeing all allocated memory.
void destroy_bag(bag_t *bag);

// Adds a webpage to the bag.
void add_to_bag(bag_t *bag, webpage_t *webpage);

// Checks if the bag is empty.
int is_empty(const bag_t *bag);

// Removes a webpage from the bag.
webpage_t *remove_from_bag(bag_t *bag);

// Checks if the bag contains a webpage with a specific URL
int contains_url(const bag_t *bag, const char *url, size_t *depth);

#endif /* BAG_H */

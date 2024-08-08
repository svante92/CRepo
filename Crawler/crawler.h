#ifndef CRAWLER_H
#define CRAWLER_H

#include <stddef.h>

typedef struct {
	char *url;
	char *html;
	size_t length;
	int depth;
} webpage_t;

typedef struct {
	// Fill in with your implementation
    	size_t capacity;
    	size_t size;
	webpage_t **webpages;
} bag_t;

typedef struct {
	// Fill in with your implementationi
	int num_slots;
	set_t **slots;
} hashtable_t;

#endif

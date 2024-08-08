#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <stdlib.h>

// Function to create a new webpage
webpage_t *webpage_new(const char *url, int depth, const char *html, size_t length);

// Function to get the URL of a webpage
const char *webpage_getURL(const webpage_t *page);

// Function to get the depth of a webpage
int webpage_getDepth(const webpage_t *page);

// Function to get the HTML content of a webpage
const char *webpage_getHTML(const webpage_t *page);

// Function to get the length of a webpage
size_t webpage_getLength(const webpage_t *page);

// Function to delete/free a webpage
void webpage_delete(webpage_t *page);

#endif // WEBPAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h> /* For uintptr_t */

#include "set.h"
#include "crawler.h"
#include "webpage.h"

/*
 * webpage.c - Provides functions to create, access, and delete webpage_t objects.
 *
 * Crawler, Svante Aretun, svante92
 */

// Function to create a new webpage
webpage_t *webpage_new(const char *url, int depth, const char *html, size_t length) {
    webpage_t *newPage = malloc(sizeof(webpage_t));
    if (newPage != NULL) {
        newPage->url = strdup(url);
        newPage->depth = depth;
        newPage->html = html == NULL ? NULL : strdup(html);
        newPage->length = length;  // Initialize the length variable
    }
    return newPage;
}

// Function to get the URL of a webpage
const char *webpage_getURL(const webpage_t *page) {
    return page->url;
}

// Function to get the depth of a webpage
int webpage_getDepth(const webpage_t *page) {
    return page->depth;
}

// Function to get the HTML content of a webpage
const char *webpage_getHTML(const webpage_t *page) {
    return page->html;
}

// Function to get the length of a webpage
size_t webpage_getLength(const webpage_t *page) {
    return page->length;
}

// Function to delete/free a webpage
void webpage_delete(webpage_t *page) {
    if (page != NULL) {
        free(page->url);
        free(page->html);
        free(page);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "set.h"
#include "crawler.h"
#include "webpage.h"
#include "pagedir.h"

/*
 * pagedir.c - Initializes the page directory and saves pages to that directory.
 *
 * Crawler, Svante Aretun, svante92
 */

bool pagedir_init(const char *pageDirectory) {
	// Fill in with your implementation
	char crawlerPath[255];  // Adjust the size as needed
    	snprintf(crawlerPath, sizeof(crawlerPath), "%s/.crawler", pageDirectory);

    	// Open the file for writing; on error, return false
    	FILE *crawlerFile = fopen(crawlerPath, "w");
    	if (crawlerFile == NULL) {
        	fprintf(stderr, "Error: Could not open .crawler file for writing in the page directory.\n");
        	return false;
    	}

	// Close the file
    	fclose(crawlerFile);

    	return true;

}

void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID) {
	// Fill in with your implementation
	char filePath[255];  // Adjust the size as needed
    	snprintf(filePath, sizeof(filePath), "%s/%d", pageDirectory, documentID);


    	// Open the file for writing; on error, print a message and return
    	FILE *file = fopen(filePath, "w");
    	if (file == NULL) {
        	fprintf(stderr, "Error: Could not open file %s for writing.\n", filePath);
        	return;
    	}

    	// Save the webpage content to the file
    	fprintf(file, "%s\n", webpage_getURL(page));
    	fprintf(file, "%d\n", webpage_getDepth(page));
    	fprintf(file, "%s\n", webpage_getHTML(page));

    	// Close the file
    	fclose(file);
}

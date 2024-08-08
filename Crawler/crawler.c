#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> /* For uintptr_t */
#include <unistd.h>

#include "set.h"
#include "crawler.h"
#include "bag.h"
#include "hashtable.h"
#include "curl.h"
#include "webpage.h"
#include "pagedir.h"
#include "url.h"

/*
 * crawler.c - Crawls the web and retrieves web pages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”.
 *
 * Crawler, Svante Aretun, svante92
 */

/**
 * Parses command-line arguments, placing the corresponding values into the pointer arguments seedURL,
 * pageDirectory and maxDepth. argc and argv should be passed in from the main function.
 */
static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth) {
	// Fill in with your implementation
	if (argc != 4) {
        fprintf(stderr, "Usage: %s <seedURL> <pageDirectory> <maxDepth>\n", argv[0]);
        exit(EXIT_FAILURE);
    	}

    	if((*seedURL = strdup(argv[1])) == NULL)
		{
        	fprintf(stderr, "Error: No memory to duplicate seedURL.\n");
        	exit(EXIT_FAILURE);
		}

    	if((*pageDirectory = strdup(argv[2])) == NULL)
		{
        	fprintf(stderr, "Error: No memory to duplicate pageDirectory.\n");
        	exit(EXIT_FAILURE);
		}

    	// Call pagedir_init to mark the page directory
    	if (!pagedir_init(*pageDirectory)) {
        	fprintf(stderr, "Error: Unable to initialize the page directory.\n");
        	exit(EXIT_FAILURE);
    	}

    	// Parse and validate maxDepth
    	char *endptr;
    	*maxDepth = strtol(argv[3], &endptr, 10);

    	if (*endptr != '\0' || *maxDepth < 0 || *maxDepth > 10) {
        	fprintf(stderr, "Error: Invalid maxDepth. It must be an integer in the range [0 ... 10].\n");
        	exit(EXIT_FAILURE);
    	}
}

/**
 * Scans a webpage for URLs.
 */
static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen, char *htmlContent) {
	// Fill in with your implementation
	int state = 0;
	char *cursor = htmlContent;
	char *pattern[] = { "<a", "href=\"", "\"" };
	char *urlStart, *urlEnd;

	while(cursor != NULL)
	{
		switch(state)
		{
			case 0:
				urlStart = urlEnd = NULL;

				/* Look for the start of the <a> tag */
				if((cursor = strstr(cursor, pattern[state])) != NULL)
				{
					cursor += strlen(pattern[state]) + 1;
					state++;
				}
				break;
			case 1:
				/* Look for the href attribute */
				if((cursor = strstr(cursor, pattern[state])) != NULL)
				{
					cursor += strlen(pattern[state]);
					urlStart = cursor;
					cursor++;
					state++;
				}
				break;
			case 2:
				/* Look for the end of the <a> tag */
				if((cursor = strstr(cursor, pattern[state])) != NULL)
				{
					urlEnd = cursor - 1;
					cursor += strlen(pattern[state]) + 1;
					state++;
				}
				break;
			default:
				/* Something went wrong, start over */
				state = 0;
				break;
		}

		/* Process once we have the url start and url end */
		if(urlStart != NULL && urlEnd != NULL)
		{
           	// Extract the URL
           	size_t urlLength = urlEnd - urlStart + 1;
           	char *url = strndup(urlStart, urlLength);

			url[urlLength] = '\0';


	
			// Normalize the URL relative to page.url
			char *normalizedURL = normalizeURL(page->url, url);

			// Free the temporary URL
			free(url);

  			if (normalizedURL != NULL)
			{
				if (isInternalURL(page->url, normalizedURL))
				{
					// Insert the webpage into the hashtable
					if (hashtable_insert(pagesSeen, normalizedURL, NULL))
					{
						// Create a webpage_t for it
						webpage_t *newPage = webpage_new(normalizedURL, page->depth + 1, NULL, 0);

						// Insert the webpage into the bag
						add_to_bag(pagesToCrawl, newPage);
						printf("%-3d%*s    Found: %s\n", page->depth, page->depth, "", webpage_getURL(newPage));
                				printf("%-3d%*s    Added: %s\n", page->depth, page->depth, "", webpage_getURL(newPage));
					}
					else
					{
						printf("%-3d%*s  IgnDupl: %s\n", page->depth, page->depth, "", normalizedURL);
					}
				}
				else
				{
					printf("%-3d%*s IgnExtrn: %s\n", page->depth, page->depth, "", normalizedURL);
				}
				free(normalizedURL);
			}

			/* Start over */
			state = 0;
		}
	}
}

/**
 * Crawls webpages given a seed URL, a page directory and a max depth.
 */
static void crawl(char *seedURL, char *pageDirectory, const int maxDepth)
{
	// Fill in with your implementation
	// Initialize the hashtable and add the seedURL
    	hashtable_t *pagesSeen = hashtable_new(150); // Adjust the size as needed
    	hashtable_insert(pagesSeen, seedURL, NULL);
		int documentID = 1;

    	// Initialize the bag and add a webpage representing the seedURL at depth 0
    	bag_t *pagesToCrawl = create_bag();
    	webpage_t *seedPage = webpage_new(seedURL, 0, NULL, 0);
    	add_to_bag(pagesToCrawl, seedPage);

    	// While the bag is not empty
    	while (!is_empty(pagesToCrawl)) {
        	// Pull a webpage from the bag
        	webpage_t *currentPage = remove_from_bag(pagesToCrawl);

        	// Fetch the HTML for that webpage
        	size_t htmlSize;
        	char *htmlContent = download(webpage_getURL(currentPage), &htmlSize);

        	if (htmlContent != NULL) {
				currentPage->html = htmlContent;
				currentPage->length = htmlSize;
            	// Save the webpage to pageDirectory
            	pagedir_save(currentPage, pageDirectory, documentID++);

            	// If the webpage is not at maxDepth, pageScan that HTML
            		if (currentPage->depth < maxDepth) {
                		printf("%-3d%*s  Fetched: %s\n", webpage_getDepth(currentPage), webpage_getDepth(currentPage), "", webpage_getURL(currentPage));
                		printf("%-3d%*s Scanning: %s\n", webpage_getDepth(currentPage), webpage_getDepth(currentPage), "", webpage_getURL(currentPage));
				pageScan(currentPage, pagesToCrawl, pagesSeen, htmlContent);
            		}

        	}	

        	// Delete that webpage
        	webpage_delete(currentPage);

			sleep(1);
    	}

    	// Delete the hashtable
    	hashtable_delete(pagesSeen, NULL);

    	// Delete the bag
    	destroy_bag(pagesToCrawl);
}

int main(const int argc, char *argv[]) {
	// Fill in with your implementation
	char *seedURL;
    	char *pageDirectory;
	int maxDepth;

    	// Parse command-line arguments
    	parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

    	// Perform crawling
    	crawl(seedURL, pageDirectory, maxDepth);

    	// Clean up and exit
    	free(seedURL);
    	free(pageDirectory);

    	return EXIT_SUCCESS;
}

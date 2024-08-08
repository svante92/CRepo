Usage: ./crawler seedURL pageDirectory maxDepth

Setup: sudo apt install libcurl4 libcurl4-gnutls-dev

Description: The program crawls the web and retrieves web pages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”.

CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lcurl
SRC = crawler.c bag.c curl.c set.c hashtable.c pagedir.c webpage.c url.c
OBJ = $(SRC:.c=.o)
DEPS = $(SRC:.c=.h)

all: crawler

crawler: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) crawler

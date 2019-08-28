# Makefile for common library with the pageDir.[ch] modules
# Builds and compiles all the utility functions into a library


# Ashley Song, CS50, 19X, Lab4
# ../libcs50/webpage.o ../libcs50/memory.o ../libcs50/file.o ../libcs50/hashtable.o ../libcs50/counters.o
OBJS = pagedir.o index.o word.o
LIB = common.a


CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make


$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)


pagdir.o: pagedir.h
index.o: index.h ../libcs50/hashtable.h
word.o: word.h
# webpage.o: ../libcs50/webpage.h
# memory.o: ../libcs50/memory.h
# file.o: ../libcs50/file.h
# hashtable.o: ../libcs50/hashtable.h
# counters.o: ../libcs50/counters.h

.PHONY: clean


#Clean after compiling
clean:
	rm -f core
	rm -f $(LIB) *~ *.o

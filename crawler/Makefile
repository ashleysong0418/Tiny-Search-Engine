
# Makefile for crawler
#
# Ashley Song, CS50, 19X, lab4
# Lab 4


PROG= crawler
OBJS= crawler.o
LIBS= ../common/common.a ../libcs50/libcs50.a 
TEST= testing.sh
# TESTING=-DMEMTEST

CFLAGS= -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC= gcc
MAKE= make

$(PROG): $(OBJS)
				$(CC) $(CFLAGS) $^ $(LIBS) -o $@

				# $(PROG): $(OBJS)
				# 				$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $(PROG)

# crawler.o: ../libcs50/hashtable.h ../libcs50/bag.h ../libcs50/webpage.h ../libcs50/memory.h
# webpage.o: ../libcs50/webpage.h
# memory.o: ../libcs50/memory.h
# hashtable.o: ../libcs50/hashtable.h
# bag.o: ../libcs50/bag.h

.PHONY: test valgrind clean


test: $(PROG)
	@./$(TEST) &>testing.out


valgrind: $(PROG)
	valgrind ./$(PROG)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

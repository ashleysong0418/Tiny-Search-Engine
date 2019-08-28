
# Makefile for Indexer
#
# Ashley Song, CS50, 19X
# Lab 5


PROG=indexer
PROG2=indextest
OBJS=indexer.o
OBJS2=indextest.o
LIBS= ../common/common.a ../libcs50/libcs50.a
TEST= testing.sh
# TESTING=-DMEMTEST

CFLAGS= -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC= gcc
MAKE= make

$(PROG): $(OBJS)
				$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(PROG2): $(OBJS2)
				$(CC) $(CFLAGS) $^ $(LIBS) -o $@

all: $(PROG) $(PROG2)
indexer.o: indexer.c

indextest.o: indextest.c


.PHONY: test valgrind clean


test: $(PROG) $(PROG2)
	@./$(TEST) &>testing.out


valgrind: $(PROG)
	valgrind ./$(PROG)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG) $(PROG2) $(TEST)

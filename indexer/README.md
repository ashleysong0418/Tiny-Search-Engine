# Indexer

###### Ashley Song, CS50, 19X, Lab5,



indexer.c module is used to parse the files created by the crawler and tracking the word count for each word recorded. It analyzes each file and stores the word and the word count for that word in each file.


##### Assumptions:
Indexer assumes that the files stored by crawler starts with the doc ID 1 and increments one at a time. Additionally, indextest extracts information from the old index file by line. As thus, we assume that the information for each word will not exceed one line.


##### Compiling:
To run indexer, first make sure the common.a library is fully compiled(as specified in the common README.md), then compile indexer using:
```
make clean; make
```
Then run the program with the two arguments-- crawler test directory path name, name of index file.

A _testing.sh_ file is also provided with various test cases with the output directly writing into a file called _testing.out_ . This file will run test cases for both indexer.c and indextest.c and eventually compare the two files. To run the test file, use this command:
```
make clean; make test
```


To run with valgrind, use this command:
```
make clean; make valgrind
```

To compile either indexer or indextest, one at a time, use this command:
```
make clean; make indexer
make clean; make indextest
```

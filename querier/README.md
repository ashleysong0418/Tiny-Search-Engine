# Querier

###### Ashley Song, CS50, 19X, Lab6



querier.c module is used to produce an output of scores in response to a search query. It utilizes the files created by crawler and indexer to do so.

Querier meets the full requirement mentioned in the lab description(ranks the scores from highest to lowest).

##### Assumptions:
We assume that each query search will be no more than 20 words. Additionally, querier runs assuming that the user will not be searching any numbers, as the program rejects and non-alphabet characters.


##### Compiling:
To run querier, first make sure the common.a and libcs50.a libraries are fully compiled(as specified in the README.md), then compile querier using:
```
make clean; make
```
Then run the program with the two arguments-- crawler test directory path name, name of index file.

A _testing.sh_ file is also provided with various test cases with the output directly writing into a file called _testing.out_ . This file will run test cases for querier.c and also uses fuzzquery.c to test random inputs. The last test case uses valgrind to verify memory leaks or errors. To run the test file, use this command:
```
make clean; make test
```


To run with valgrind, use this command:
```
make clean; make valgrind
```

To compile either querier.c or fuzzquery.c, one at a time, use this command:
```
make clean; make querier
make clean; make fuzzquery
```

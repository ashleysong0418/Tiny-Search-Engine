# Crawler

###### Ashley Song, CS50, 19X, Lab4, 



crawler.c module is used to crawl the web starting with a given seed url for a max depth that is specified by the user.


##### Assumptions:
In the crawler function, the hashtable was created with the assumption of only needing 100 slots. The testing showed that 100 slots was enough. Additionally, crawler was made with the assumption that the maximum depth will be greater than zero, but less than 10.


##### Compiling:
To run crawler, first make sure the common.a library is fully compiled(as specified in the common README.md), then compile crawler using:
```
make clean; make
```
Then run the program with the three arguments-- seed url, directory, and max depth.

A _testing.sh_ file is also provided with various test cases with the output directly writing into a file called _testing.out_ . To run the test file, use this command:
```
make clean; make test
```
This is assuming that _testing.out_ is an existing, writable file.

To run with valgrind, use this command:
```
make clean; make valgrind
```

Please note that the _testing.sh_ file holds urls that will run for a long time.

The _crawlertestdir_ is the directory that crawler will save to. It currently holds three sample files as a result of running
```
./crawler  http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html _crawlertestdir_ 2
```

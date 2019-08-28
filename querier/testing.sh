# Ashley Song, CS50, 19X, Lab 6 Querier,
# This file is for the purpose running various test cases for the querier.c
# program. Below are various bash commands that will run the program and save
# the output to a new file called testing.out.
#
# This test can also be run with the command line arguments: make test
#

# Note: the last test case assumes that fuzzquerier has been created and compile exactly how it was shown in the lecture notes



echo "Test 1: Querier, Using too few arguments:"
./querier ../indexer/indexFile

echo "Test 2: Querier, Using too few arguments (no file name):"
./querier ../crawler/crawlertestdir/

echo "Test 3: Querier, Using too few arguments (no directory path):"
./querier ../indexer/indexFile

echo "Test 4: Querier, Non-existing directory:"
./querier ../crawler/notreal ../indexer/indexFile

echo "Test 5: Querier, Non-existing file:"
./querier ../crawler/crawlertestdir/ ../indexer/notreal

echo "Test 6: Querier, Correct arguments, using fuzzquery for random query inputs (using valgrind to check for memory leaks):"
./fuzzquery ../data/index.letters 10 0 | ./querier ../data/letters ../data/index.letters

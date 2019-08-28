# Ashley Song, CS50, 19X, Lab 4 Crawler,
# This file is for the purpose running various test cases for the crawler.c
# program. Below are various bash commands that will run the program and save
# the output to a new file called testing.out.
#
# This test can also be run with the command line arguments: make test
#

# NOTE: For some of the URLs, the larger depth means a longer run-time. The tests
# are to show that the program works without errors or memory issues. However, for
# this entire testing.sh to finish running, it will take a very long time.

# The different URLs:
# URL 1: http://old-www.cs.dartmouth.edu/~cs50/index.html
# URL 2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
# URL 3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html
# URL 4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html

echo "Test 1: Valid Input URL 1"
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html crawlertestdir/ 2

echo "Test 2: Bigger maxDepth URL 1"
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html crawlertestdir/ 8

echo "Test 3: Non-Existing Directory"
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html notreal/ 2

echo "Test 4: Wrong Number of Arguments"
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html crawlertestdir/

echo "Test 5: Non-Existent URL"
./crawler thisisnotrealurl crawlertestdir/ 2

echo "Test 6: Non-Internal URL"
./crawler http://google.com crawlertestdir/ 2

echo "Test 7: Existent URL but Non-Existent Page URL"
./crawler http://old-www.cs.dartmouth.edu/~cs50/index1.html crawlertestdir/ 2
#(This test will say all parameters valid, but will fail with webpage_fetch)

echo "Test 8: Out of Depth Range (above 10)"
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html crawlertestdir/ 12

echo "Test 9: Out of Depth Range (less than 0)"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html -1

echo "Test 10: More than 4 arguments"
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html crawlertestdir/ 2 extra

echo "Test 11: Valid Input URL 2"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html crawlertestdir/ 2

echo "Test 12: Bigger maxDepth URL 2"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html crawlertestdir/ 8

echo "Test 13: Valid Input URL 3"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html crawlertestdir/ 2

echo "Test 14: Bigger maxDepth URL 3"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html crawlertestdir/ 8

echo "Test 15: Valid Input URL 4"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html crawlertestdir/ 2

echo "Test 16: Bigger maxDepth URL 4"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html crawlertestdir/ 8

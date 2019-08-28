# TSE Querier Design Spec
###### Ashley Song, CS50, 19X, lab6

The querier.c program is responsible for taking the files created by the **crawler** and **indexer** and parsing a query search for to output a search result. Querier is designed to look over one word at a time to formulate a final score. The program excludes any "and"/"or" during the querying. Query uses the files created by **crawler** to extract the URL during printing and also uses the index created by **indexer** to check that a word within the query input is in the files extracted by **crawler**.


### User interface

Querier's interface is the command line and the standard input. User will start program with usage specified below, and then input query searches in the stdin once the program is started.

```
./querier <crawler directory file path name> <file name for the index output>
```

For example:

``` bash
$ ./querier ../crawler/crawlertestdir indexFile
```

### Inputs and outputs

Input: user puts in a string of words with optional "and"/"or" statements for the search

Output: the scores, docIDs, and URLs for the respective word(s), ranked in order from highest to lowest score

Example:
```
Clean query: eniac or first or home and home and this
Score		2  Doc		5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
Score		2  Doc		4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
Score		2  Doc		6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
Score		2  Doc		1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Score		2  Doc		3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Score		1  Doc		2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Score		1  Doc		7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
```



### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *querierFunc*, which is responsible for handling all other functions for parsing input and producing "clean query" as well as producing the results
 3. *runQuery*, which gathers the scores by parsing the query input
 4. *sortQueryResults*, which sorts the final scores in descending order
 4. *printQueryResults*, which prints the results
 5. *normalizeWord*, which loops over each character in a word and converts it to the lower case

And some helper modules that provide data structures:

 1. *index*, a wrapper struct for the hashtable data structure, specific for the indexer
 2. *counters*, the set-like data structure that is stored in the hashtable for the purpose of tracking the count for each word

 And some new *structs*:
 1. ```struct twocts```, which holds two counters during intersections
 2. ```struct idScore```, which holds score/id pairs during sorting and printing


### Pseudo code for logic/algorithmic flow

The querier will run as follows:

1. parse the arguments to make sure all files and directories are existing and readable.
2. While the user is still inputting queries,
  * parse the query to check for "bad" characters
  * tokenize the query input for any whitespace
  * validate the query input, making sure no "and"/"or" are at the beginning or end of the line and are not adjacent to each other
  * print the clean query
  3. run the query:
    * make sure word is in the index
    * intersect words until an "or", ignoring any "ands"
    * unionize each intersection for every "or"
    * return the final scores counter
  4. sort the scores:
    * extract the scores and docIDs from the final counter
    * sort the scores from highest to lowest
  5. print the scores, ids, and URLs
    * get the URLs for each doc from the crawler files
  6. free data to move onto next query search
7. clean up: free all data storage before quitting program


**normalize the word** means to convert each letter in the word to a lower case.
As one simple example,
`Home`
becomes
`home`.


For cleaner implementation, the code in this repository does not follow the exact pseudocode above. Rather, it breaks each of the large components into separate functions.


The querier completes and exits when it the user stops inputting more query searches (control D ends the input).

### Assumptions:
We assume that each query search will be no more than 20 words. Additionally, querier runs assuming that the user will not be searching any numbers, as the program rejects and non-alphabet characters.


### Dataflow through modules

 1. *main* parses parameters and initializes/loads the index and later frees it too.
 2. *querierFunc* expects a counter_t from the *runQuery*, and holds words in an array of pointers. *runQuery* initializes and deletes counters as necessary.
 3. *sortQueryResults* iterates through the counter and produces an array of struct idScores.
 4. *printQueryResults* takes a the array of structs and frees the urls produced by *getURL*.


### Major data structures

Three helper modules provide data structures:

 1. *set* of URLs (indirectly used by hashtable)
 2. *hashtable* of counters identified by key values set as words (aka *index*)
 3. *counters* of doc ID and word count

### Testing plan

*Integration testing*.  Assemble the indexer and test it as a whole.
In each case, examine the output files carefully to be sure they have the correct words, doc IDs and counts.
Ensure that no words or counts are missing or duplicated.
Test with a shorter file for easier analysis.

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

0. Test the querier with a non-existent directory.

0. Test the querier with too few arguments.

0. Test the querier with too many arguments.

1. Test the querier with fuzzquery.c to test random query searches. This ensures no edge cases are missed. 

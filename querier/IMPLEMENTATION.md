# TSE Querier Implementation Specs
###### Ashley Song, CS50, 19X, lab6

## Overview

The querier.c program is responsible for taking the files created by the **crawler** and **indexer** and parsing a query search for to output a search result. Querier is designed to look over one word at a time to formulate a final score. The program excludes any "and"/"or" during the querying. Query uses the files created by **crawler** to extract the URL during printing and also uses the index created by **indexer** to check that a word within the query input is in the files extracted by **crawler**.

#### Important Definitions:
* **normalizeWord**: this means to turn each character in a word to a lower case.
* **index file**: this is the file being created by the indexer to compile each word count and the doc ID that the word is found in.
* **Intersect/Intersection**: any time the word "and" is used in the query input, the user is indicating that they want the files that have **ALL** the words in the "and" sequence. Therefore, the program finds the "intersection" of files for the words and returns the minimum word count.
* **Union/Unionizing**: any time the word "or" is used in the query input, the user is indicating that they want the files that have at least one of the different "and sequences". Therefore, the program returns the score of the sum of word counts and any files that hold the sequence of words.

  * Think of the intersect/union concept as the [Backus-Naur Form](https://en.wikipedia.org/wiki/Backus–Naur_Form). It is similar to the concept of PEMDAS in math.

* **Tokenizing/tokens**: Think of the query as a sequence of *tokens*, each a `word` or a literal ('and', 'or'), with space(s) between tokens, and optional space at the beginning or end of the line. (Definition taken from REQUIREMENTS.md)

* **Score**:
    Because we want to rank the documents satisfying the query, we need to score each document by *how well* it satisfies the document.

    The score for a document satisfying a single-word query for `word` is the number of occurrences of `word` in that document.
    (Fortunately, that's what your index tells you.)

    The score for a document satisfying a conjunction (aka `andsequence`) `wordA and wordB` is the *minimum* of the score for `wordA` and the score for `wordB` on this document.
    By induction, the score for a document satisfying `<andsequence> and wordC` is the minimum of the score for the `andsequence` and the score for `wordC` on this document.

    The score for a document satisfying a disjunction `wordA or wordB` is the *sum* of the score for `wordA` and the score for `wordB` on this document.
    By induction, the score for a document satisfying `<andsequenceA> or <andsequenceB>` is the sum of the score for `andsequenceA` and the score for `andsequenceB` on this document.


#### Data Structures:
Querier mainly uses two different data structures, but some of them are abstracted out.

* ##### index_t
~ The index data structure is one that wraps the hashtable class and uses the hashtable functions. However, index specifies the parameters to fit exactly what indexer will be using. For example, the `void *item` in hashtable will be specified as a  `counter_t` type by index. Index abstracts the functionality of the hashtable and uses it more specifically. Refer to index.h file for specified information on each function in the module.

* ##### hashtable
~ A hashtable is a set of _key/item_ pairs and acts like the set module. The hashtable module, however, enables more efficient lookup of the _key/item_ pairs due to the _hashkey_ value that is calculated using the jhash.h module. A _hashtable_t_ holds and array of pointers to _set_t_ structs. Duplicates are not allowed and the items are not stored in order. A hashtable is initialized with a given slot size. Refer to the hashtable.h file for specified information on each function in the module.

* ##### set
~ A set is an unordered collection of *unique* _key/item_ pairs. This is also implemented as a linked list. However, the user cannot remove or update pairs. The set_t struct holds the pointer to the head of the linked list while each setnode_t holds the pointers to its item, key, and the next node. While the set data structure was not explicitly used, the hashtable module is using the set structure. Refer to the set.h file for specified information on each function in the module.

* ##### counter
~ The counter module is implemented like a set. Instead of holding items, however, counter keeps track of the count for each key. Querier uses counter to keep track of the score. Refer to the counters.h file for specified information on each function in the module.


#### Querier Function Prototypes
```
void querierFunc(char* input, index_t *index, char* dirName);
```
* The querierFun function acts like the second "main" function. It is responsible for calling helper functions in tokenizing and creating the "clean" query. It later calls the other functions to actually parse the query for the scores, sort the scores, and print.

```
counters_t* runQuery(index_t *index, char *words[20], int numWords, int orCount);
```
* runQuery is called by querierFunc to parse through the query input and calculate the scores for the output. It calls on other `static` functions such as counters_intersect and counters_union to calculate. This function returns a counter that holds the final scores and docIDs to querierFunc.

```
struct idScore* sortQueryResults(counters_t *finalSum, struct idScore *array);
```
* sortQueryResults is the function that is responsible for sorting the final scores in descending order. It calls on helper functions to do so. To sort, the scores and docIDs are extracted from the counter and stored as a pair in `struct idScore` and put into an array. The array of `struct idScore` is sorted.

```
void printQueryResults(struct idScore *array, char* pageDir);
```
* printQueryResults is the function that will finally print the results for the user. It calls on helper functions to extract the URL of each docID from the crawler produced files, and then prints the score, ID, and the URL.


#### Libraries:
* The common.a library holds the functions used by **crawler** and **indexer**. No new functions were added specifically for querier, but querier utilizes the functions and modules in common.a and libcs50.a. `normalizeWord` is just one example of some of the functions that querier draws from the libraries.



#### Storage and Memory Handling:
* In querier, an index_t is loaded and freed after the user is done with all inputs. Counters_t are created and freed throughout the program. Any arrays used do not malloc and space, so they do not need to be freed.

#### Error Handling
* Throughout indexer.c, the various `fprintf` statements were strategically placed around the functions to track the errors and `NULL` returns. Additionally, `exit` statements are left throughout the main during the parsing of the input argument.

#### Error Testing
* Using `valgrind` and `gdb`, all memory leaks were identified and fixed.


### Pseudocode for Querier:
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



Note: For cleaner implementation, the code in this repository does not follow the exact pseudocode above. Rather, it breaks each of the large components into separate functions and many static helper functions.



#### Testing:
* fuzzquery.c was used to produce random query searches:

```
$ crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data/letters 3
$ indexer/indexer data/letters data/index.letters
$ querier/fuzzquery data/index.letters 10 0 | querier/querier data/letters data/index.letters
```

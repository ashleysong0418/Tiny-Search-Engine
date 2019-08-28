# TSE Crawler Implementation Specs
###### Ashley Song, CS50, 19X, lab4

## Overview

The Crawler.c program is responsible for crawling the web by starting with a "seed" URL and retrieving webpages on each page. This program starts by parsing the webpage given by the "seed" URL and "crawls" the page to extract other URLs embedded on the page. Crawler recursively extracts the URLs until the maximum "depth" is reached.

#### Important Definitions:
* Depth: if given a `maxDepth` of 1, crawler will only explore the URLs that are on the `seedURL` page and the pages linked to the `seedURL`. With a `maxDepth` of 2, crawler will explore the pages linked to the first pages of depth 1. And so on.
*  Seed URL: this is the starting point URL for the crawler, given by user input.
* pageDirectory: is the directory that holds the files that crawler will create and write the output to. Make sure this is an existing and accesible directory.

#### Data Structures:
Crawler uses three different data structures, one of which is not used visibly.

* ##### bag
~ A bag is an unordered collection of `void *items` that is initialized to be empty but grows as more `items` are added to the bag. Bag is implemented as a linked list. As the items in the bag are of non-specified type, it will extract any item from the bag. Bag allows duplicates to be added as no key value is stored with the items. The bag `struct` is of type _bag_t_, which will hold each _bagnode_t_. The _bagnode_t_ holds the item itself. The bag data structure is in crawler as the data structure to hold the _webpage_t_ structs. Refer to bag.h file for specified information on each function in the module.

* ##### hashtable
~ A hashtable is a set of _key/item_ pairs and acts like the set module. The hashtable module, however, enables more efficient lookup of the _key/item_ pairs due to the _hashkey_ value that is calculated using the jhash.h module. A _hashtable_t_ holds and array of pointers to _set_t_ structs. Duplicates are not allowed and the items are not stored in order. A hashtable is initialized with a given slot size. Refer to the hashtable.h file for specified information on each function in the module.

* ##### set
~ A set is an unordered collection of *unique* _key/item_ pairs. This is also implemented as a linked list. However, the user cannot remove or update pairs. The set_t struct holds the pointer to the head of the linked list while each setnode_t holds the pointers to its item, key, and the next node. While the set data structure was not explicitly used in crawler, the hashtable module is using the set structure. Refer to the set.h file for specified information on each function in the module.

* ##### webpage
~ The webpage module is used directly by crawler to store the page URL, depth, and HTMl. It is stored in the bag as an item for exploration. Refer to the webpage.h file for specified information on each function in the module.

#### Crawler Function Prototypes
```
void crawler(char *urlSeed, char *pageDir, int maxDepth);
```
* The crawler is the function that is responsible for exploring each page for the URLs. It takes in a pointer to the `seedURL`, the pointer to the name of the page directory, and the maxDepth. These are the arguments given by the user when running the compiled crawler.

```
void pageDelete(void *page);
```
* This is the helper function passed off to the bag_delete function. It is responsible for freeing each item in the bag. A pointer to the item is passed in as a parameter.

```
void hashDelete(void *item);
```
* This is the helper function passed off to the hashtable_delete function. Unlike the pageDelete function, hashDelete does not do anything. It is just an empty function passed into hashtable_delete to fulfill the necessary parameters.

```
inline static void logr(const char *word, const int depth, const char *url);
```
* The logr function is one that was given in the CS50 lecture. It prints statements to show when a webpage was scanned, found, fetched, etc. It is a function used to keep track of what the function is doing and at what stage the URLs are at.

#### pageDir.a Library:
```
bool webpage_save(webpage_t *page, char *pageDir, int id);
```
* The webpage_save function is responsible for taking each webpage after the HTML was successfully fetched and saves the URL, depth, and the HTML into a file in the given directory. Each file is named after a unique id.

```
bool isPageDir(char **pageDir);
```
* The isPageDir function checks to see if a given argument is an existing directory, return true if it is existing and false if it not. This function takes a double pointer to the directory name.


```
bool hasWritePermission(char *pageDir)
```
* The hasWritePermission takes a char pointer to the directory name and returns true if the directory has write permission. Otherwise, it returns false.

#### Storage and Memory Handling:
* Throughout the crawler function, each extracted URL is stored in a hashtable to keep track of which URLs have already been visited. The URL is also added in a webpage_t struct, then added to the bag to later be explored. Each extracted URL memory is freed if not added to the bag. After full exploration, the webpage_t is freed from memory. At the end of the crawl, the hashtable and bag are also freed using the hastable_delete and bag_delete function. The directory given by the user will hold the files for each saved URL.

#### Error Handling
* Throughout crawler.c, the logr function is used to log when a URL has been fetched, found, added, or ignored. This method was used to track that the crawler function was exploring the URLs correctly. Additionally, `exit` statements are left throughout the main during the parsing of the input argument. `fprintf` functions are also strategically placed around the crawler function as well.

#### Error Testing
* Using `valgrind` and `gdb`, all memory leaks were identified and fixed.


### Pseudocode for Crawler:

* In main function, check that there is the correct number of arguemnts (`argc = 4`).
* Check that the url is valid
* Check that the directory exists and is writable
* check that the last argument is an integer and is in the range of [0,10]
* pass off the arguments to crawler function
* initialize bag and hashtable
* initialize first webpage_t struct with seed url
* insert seed into bag
* while bag is not empty,
  * fetch the html
  * save the page to Directory
  * if the depth is less than the max depth
    * while there is another url to extract from the page
      * normalize the url
      * check that it is internal
        * if not, free the url
      * if it is internal, insert url into hashtable with other already visited urls
        * create new webpage_t struct with incremented depth
        * insert new webpage_t into bag
      * if url already visited, ignore
    * free the current webpage being explored and move onto next page in bag
* delete the bag and the hashtable

### Pseudocode for webpage_save

* allocate memory for the path name
* copy the parameter into the path name variable
* create a buffer for the file name and put the id value into it
* concatenate the id file name with the path name
* if the page and the directory are not null and the id is greater than zero,
  * open the file
  * if the file is not null, print the url, depth, and the html into the file.
  * close the file
  * return true if successful, false otherwise.


#### URLs used for test cases:
* http://old-www.cs.dartmouth.edu/~cs50/index.html
* http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
* http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html
* http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html
  _Note that the last two urls take particularly long to fully crawl, expecially with a larger depth._

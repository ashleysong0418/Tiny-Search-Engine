# TSE Indexer Implementation Specs
###### Ashley Song, CS50, 19X, lab5

## Overview

The indexer.c program is responsible taking the files created by the **crawler** and parsing the file to collect data for storage in a data structure. From each file, **indexer** will gather the URL, depth, and the HTML to re-create a webpage struct for the purpose of parsing through the words in the document. **indexer**, through this process, will store the word count for each word in each document, later compiling the data and printing it to a file.

#### Important Definitions:
* normalizeWord: this means to turn each character in a word to a lower case.
* index file: this is the file being created by the indexer, and later compared to the file created by indextest.

#### Data Structures:
Indexer uses three different data structures, some of which are abstracted out.

* ##### index
~ The index data structure is one that wraps the hashtable class and uses the hashtable functions. However, index specifies the parameters to fit exactly what indexer will be using. For example, the `void *item` in hashtable will be specified as a  `counter_t` type by index. Index abstracts the functionality of the hashtable and uses it more specifically. Refer to index.h file for specified information on each function in the module.

* ##### hashtable
~ A hashtable is a set of _key/item_ pairs and acts like the set module. The hashtable module, however, enables more efficient lookup of the _key/item_ pairs due to the _hashkey_ value that is calculated using the jhash.h module. A _hashtable_t_ holds and array of pointers to _set_t_ structs. Duplicates are not allowed and the items are not stored in order. A hashtable is initialized with a given slot size. Refer to the hashtable.h file for specified information on each function in the module.

* ##### set
~ A set is an unordered collection of *unique* _key/item_ pairs. This is also implemented as a linked list. However, the user cannot remove or update pairs. The set_t struct holds the pointer to the head of the linked list while each setnode_t holds the pointers to its item, key, and the next node. While the set data structure was not explicitly used in crawler, the hashtable module is using the set structure. Refer to the set.h file for specified information on each function in the module.

* ##### webpage
~ The webpage module is used directly by crawler to store the page URL, depth, and HTMl. It is stored in the bag as an item for exploration. Refer to the webpage.h file for specified information on each function in the module.

* ##### counter
~ The counter module is implemented like a set. Instead of holding items, however, counter keeps track of the count for each key. Indexer uses counter to keep track of word count for each word in each file. It is inserted into the hashtable as the item. Refer to the counters.h file for specified information on each function in the module.


#### indexer Function Prototypes
```
index_t *indexBuild(char *dir);
```
* The indexBuild function is responsible for creating the index and requires a pointer to the crawler test directory name. It initializes the data structures and returns a completed index. indexBuild calls other helper functions. These helper functions are described below.

```
static bool wordCollect(webpage_t *page, index_t *index, const int id);
```
* wordCollect is called by indexBuild to parse through each page and extract each word. Here, each word is normalized by the `normalizeWord` function. wordCollect takes a pointer to the webpage_t struct, the index, and the document id.

```
void indexSave(index_t *index, char *file);
```
* indexSave is the function that is responsible for taking the contents of the index and printing it out to the file. indexSave will call other helper functions to iterate over each counter in the index. 


#### common.a Library:

```
webpage_t* webpage_recreate(FILE *fp);
```
* This function is responsible for parsing each crawler directory test file and creating the webpage_t struct, which is passed into wordCollect. It parses the first line for the URL, the second line for the depth, and the rest of the file for the HTML. This function takes a file pointer and returns a webpage_t struct.

```
void hashDelete(void *item);
```
* This is the helper function passed off to the index_delete function. This function will free each counter in the index.



#### Storage and Memory Handling:
* Throughout the indexer, each extracted word, doc ID, and count are stored in an index_t struct. During this process, a webpage_t struct is built for the word extraction process; it is freed after. After full parsing of the files, the index_t contents are printed to a file and the index_t itself is deleted with its contents.

#### Error Handling
* Throughout indexer.c, the various `fprintf` statements were strategically placed around the functions to track the errors and `NULL` returns. Additionally, `exit` statements are left throughout the main during the parsing of the input argument.

#### Error Testing
* Using `valgrind` and `gdb`, all memory leaks were identified and fixed.


### Pseudocode for indexer:

1. validate the given arguments: check if directory exists and is readable, check if file has write permission or create the file if it doesn't already exist
2. initialize the data structures
3. build the index:
  4. for each crawler directory file,
    5. gather the file's info and create the webpage_t struct for that page
    6. collect the words in the page:
      7. for each word in the page, if the word is not less than 3:
        8. normalize the word
        9. if the word is in the index, increment the count
        10. if it is not in the index yet, add it to the index with a new counter
11. after all files have been parsed, save the index to the file according to the format described above
12. delete all initialized data structures

Note: For cleaner implementation, the code in this repository does not follow the exact pseudocode above. Rather, it breaks each of the large components into separate functions such as index_build and index_save.



#### Files used for testing:
* Three crawler directory files were used for testing. Those files were created from the seedURL: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html

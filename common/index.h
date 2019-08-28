/*
* index.h header file for pagedir module
*
*
*
* Ashley Song, CS50, 19X Lab4
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "pagedir.h"
#include "../libcs50/webpage.h"

/*********************** index_t data structure ***************/
typedef hashtable_t index_t;


/* index_new is responsible for creating an index_t data structure
This function uses the hashtable data structure and casts it to an index_t struct

Input: number of slots needed in index_t
Output: a pointer to the newly initialized index_t
*/
index_t *index_new(const int num_slots);


/* index_insert is responsible for adding a new counter to the index_t
This function uses the hashtable data structure and casts it to an index_t struct

Input: a pointer to the index_t, a pointer to a key, and a pointer to an already initialized
      counter.
Output: false if key exists in index_t, any parameter is NULL, or error
        true iff new counter was inserted.
*/
bool index_insert(index_t *index, const char *key, counters_t *counter);


/* index_find is responsible for finding a counter in the index_t

Input: pointer to the index_t and the key
Output: NULL if hashtable is NULL, key is NULL, or key is not found.
        pointer to the counter corresponding to the given key, if found;

*/
counters_t *index_find(index_t *index, const char *key);

/* index_print is responsible for iterating through the index and printing the contents.

Input: pointer to the index_t and the file
Output: void.
Note: This function is used only for error and testing purposes
*/
void index_print(index_t *index, FILE *fp);

/* index_delete is responsible for deleting the contents of the index and the index_t
itself.
This function calls the hashtable implmentation of the delete function
Input: pointer to index, a pointer to a helper delete function, specifically to
      free the counters
Output: none
*/
void index_delete(index_t *index, void (*itemdelete)(void *counter));

/*****************************************************************/
/******************* Indexer and Indextest functions *************/

/* indexSave is responsible for taking the index_t and printing it to a file
This function utilizes two static helper functions for printing.

Input: pointer to the index_t, and the file name;
Output: nothing, check the file to make sure function printed correctly.

Note: this function should be printing one word on each line, followed by
      docID and count pairs
*/
void indexSave(index_t *index, char *file);

/* indexLoad is responsible for taking a file that the indexBuild function created
and re-creating the index from the file.

Input: a pointer to the already existing index file name
Output: a pointer to the newly created index

Note: after using this function, and then printing the new index to a new file,
      the output should be the same, except the order of the lines might be different.
      Use the sorting code to sort both files alphabetically, then compare.

*/

index_t *indexLoad(char *oldIndexer);

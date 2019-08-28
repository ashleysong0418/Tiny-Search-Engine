/* Ashley Song, CS50, 19X
*  Lab 5, Indexer and Indextest functions,
* This file holds the functions called by indexer and indextest as well as the index_t struct module functions. See index.h for explanations on each function.
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "pagedir.h"
#include "index.h"
#include "../libcs50/webpage.h"


void indexSave(hashtable_t *ht, char *file);
static void indexPrint(void *arg, const char *word, void *counter);
static void countPrint(void *arg, const int id, const int count);
static void indexIteratePrint(FILE *fp, const char *key, void *counter);
index_t *indexLoad(char *oldIndexer);

/*********************** index_t data structure ***************/
typedef hashtable_t index_t;

index_t *index_new(const int num_slots){
  return (index_t*)hashtable_new(num_slots);
}

bool index_insert(index_t *index, const char *key, counters_t *counter){
  return (index_t*)hashtable_insert(index, key, counter);
}

counters_t *index_find(index_t *index, const char *key){
  return hashtable_find(index, key);
}


void index_print(index_t *index, FILE *fp){
  hashtable_print(index, fp, indexIteratePrint);
}


void index_delete(index_t *index, void (*itemdelete)(void *counter)){
  hashtable_delete(index, itemdelete);
}

/*****************************************************************/
/******************* Indexer and Indextest functions *************/

static void indexIteratePrint(FILE *fp, const char *key, void *counter){
  if(counter != NULL){
    counters_t* theCounter = counter;
    counters_print(theCounter, fp);
  }
}

void indexSave(index_t *index, char *file){
  FILE *fp;
  fp = fopen(file, "w");
  if(fp != NULL){
    hashtable_iterate(index, fp, indexPrint);
    fclose(fp);
  }
}

static void indexPrint(void *arg, const char *word, void *counter){
  counters_t *count = counter;
  FILE *fp = arg;
  if(word != NULL && fp != NULL){
    fprintf(fp, "%s", word);
    counters_iterate(count, fp, countPrint);
    fprintf(fp, "\n");
  }
}

static void countPrint(void *arg, const int id, const int count){
  FILE *fp = arg;
  int docID = id;
  int wordCount = count;

  if(fp != NULL && docID > 0 && wordCount > 0){
    fprintf(fp, " %d %d", docID, wordCount);
  }
}


index_t *indexLoad(char *oldIndexer){
  FILE *fp = fopen(oldIndexer, "r");

  int numLines = lines_in_file(fp); //count the number of lines to create right size index_t
  index_t *index = index_new(numLines);
  char *word;
  int docID;
  int count;

  while((word = freadwordp(fp)) != NULL){   //get the word at the beginning of each line
    counters_t *newCounter = counters_new();  //create a new counterSet for each word
    index_insert(index, word, newCounter);
    while(fscanf(fp, "%d %d", &docID, &count) == 2){
      //if docID is already in the counterSet, it will just set that counter to the
      //count value. If it is not already in the counterSet, it will add a new counter with that count value
      counters_set(newCounter, docID, count);
    }
    count_free(word);
  }
  fclose(fp);
  return index;
}

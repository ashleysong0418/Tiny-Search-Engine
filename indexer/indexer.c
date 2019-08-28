/* Ashley Song, CS50, 19X
*  Lab 5, Indexer,
*
* The indexer is responsible taking the files created by the **crawler** and parsing the file to collect data for storage in a data structure. Within the scope
* of the TSE, indexer is finding the word count, later to be used by querier. Indexer uses a index_t struct for data storage and handling.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "webpage.h"
#include "memory.h"
#include "hashtable.h"
#include "counters.h"
#include "pagedir.h"
#include "index.h"
#include "word.c"

index_t *indexBuild(char *dir);
static bool wordCollect(webpage_t *page, index_t *index, const int id);
void hashDelete(void *item);




int main(const int argc, char *argv[]){
  char* program = argv[0];

  //check for correct number of arguments
  if(argc != 3){
    fprintf(stderr, "usage: <%s> <directory pathname> <index filename>\n", program);
    exit(1);
  }

  char *pageDir = argv[1];
  char *fileName = argv[2];
  //check if directory exists
  if((isPageDir(&pageDir)) == false){
    exit(2);
  }

  //check if directory has read permissions
  //check if fileName has write permission -- if it doesn't exist, can it be written?
  if(hasReadPermission(pageDir) == false){
    exit(3);
  }

  if(hasWritePermission(fileName) == false){
    //if the file does not exist, create the file and then check that it has write permission again
    FILE* file = fopen(fileName, "w");
    fclose(file);
  }

  index_t *index = indexBuild(pageDir);
  indexSave(index, fileName);
  index_delete(index, hashDelete);

  exit(0);
}

index_t *indexBuild(char *dir){
  FILE *fp;
  webpage_t *page;

  //initialize data structures: hashtable
  index_t *index = index_new(800);

  //create path name string
  int fileID = 1;
  char file[(strlen(dir) + 5)];
  sprintf(file, "%s/%d", dir, fileID);

  while((fp =fopen(file, "r")) != NULL){
    page = webpage_recreate(fp);

    if(wordCollect(page, index, fileID) == false){
      fprintf(stderr, "Words could not be examined in file: %d\n", fileID);
    }

    // webpage_delete(page);
    fclose(fp);
    sprintf(file, "%s/%d", dir, ++fileID);
  }
  return index;

}

static bool wordCollect(webpage_t *page, index_t *index, const int id){
  int pos = 0;
  char *result = NULL;
  counters_t *wordCounter = NULL;
  if(page == NULL || index == NULL || id <= 0){
    webpage_delete(page);
    return false;
  }
  while((result = webpage_getNextWord(page, &pos)) != NULL){
    if(strlen(result) >= 3){
      //normalize the word
      // for (int i = 0; i < strlen(result); i++) result[i] = tolower(result[i]);
      normalizeWord(result);
      //if the word is in the index, increment count
      if((wordCounter = index_find(index, result)) != NULL){
        counters_add(wordCounter, id);
      }
      else{
        //if the word is not in the index, add new
        counters_t *newCounter = counters_new();
        counters_add(newCounter, id);
        index_insert(index, result, newCounter);
      }
    }
    count_free(result);
  }

  webpage_delete(page);
  return true;
}

void hashDelete(void *item){    //as seen in crawler.c
  if(item != NULL){
    counters_delete(item);
  }
}

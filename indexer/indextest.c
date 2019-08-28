/* Ashley Song, CS50, 19X
*  Lab 5, Indexer: indextest.c
*
*
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include "webpage.h"
#include "memory.h"
#include "hashtable.h"
#include "counters.h"
#include "pagedir.h"
#include "index.h"
#include "file.h"

index_t *indexLoad(char *oldIndexer);
void hashDelete(void *item);

int main(const int argc, char *argv[]){
  char* program = argv[0];
  char* oldIndex = argv[1];
  char* newIndex = argv[2];

  //check for correct number of arguments
  if(argc != 3){
    fprintf(stderr, "usage: <%s> <old index filename> <new index filename>\n", program);
    exit(1);
  }

  //check if oldIndex has read permissions
  if(hasReadPermission(oldIndex) == false){
    exit(2);
  }

  if(hasWritePermission(newIndex) == false){
    //if the file does not exist, create the file and then check that it has write permission again
    FILE* file = fopen(newIndex, "w");
    if(file == NULL){
      fprintf(stderr, "Failed to create newIndex file\n");
      exit(3);
    }
    fclose(file);
  }

  //load up the index from data in oldIndex file
  index_t *index = indexLoad(oldIndex);
  //save the index to a new file
  indexSave(index, newIndex);
  index_delete(index, hashDelete);
  exit(0);

}

void hashDelete(void *item){    //as seen in crawler.c
  if(item != NULL){
    counters_delete(item);
  }
}

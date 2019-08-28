/*
* pagedir.cs
* All functions that are related to accessing a directory.
* Refer to .h file for further information
*
* Ashley Song, CS50, 19X, Lab4
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include "../libcs50/memory.h"
#include "pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"




bool isPageDir(char **pageDir){
  DIR* dir = opendir(*pageDir);
  if (dir) {
    /* Directory exists. */
    closedir(dir);
    return true;
  } else if (ENOENT == errno) {
    fprintf(stderr, "This directory does not exist\n");
    return false;
  } else {
    // opendir() failed for different reason
    fprintf(stderr, "Opening directory has failed\n");
    return false;

  }
}

bool isCrawlerDir(char *dirCrawler){
  char *pathName = count_malloc(strlen(dirCrawler)*2);
  assert(pathName != NULL);
  strcpy(pathName, dirCrawler);
  char file[2*(strlen(dirCrawler))];
  sprintf(file, "/.crawler");
  strcat(pathName, file);
  if(access(pathName, F_OK) !=0){
    fprintf(stderr, "This directory is not Crawler produced\n");
    count_free(pathName);
    return false;
  }
  count_free(pathName);
  return true;
}

bool isFile(char *fileName){
  if(access(fileName, F_OK) !=0){
    fprintf(stderr, "This file does not exist\n");
    return false;
  }
  return true;
}

bool hasWritePermission(char *pageDir){
  if(access(pageDir, W_OK) != 0){
    fprintf(stderr, "This does not have correct write permission\n");
    return false;
  }
  return true;
}

bool hasReadPermission(char *pageDir){
  if(access(pageDir, R_OK) != 0){
    fprintf(stderr, "This does not have correct read permission\n");
    return false;
  }
  return true;
}


bool webpage_save(webpage_t *page, char *pageDir, int id){
  char *pathName = count_malloc(strlen(pageDir)*2);
  assert(pathName != NULL);
  strcpy(pathName, pageDir);
  char file[2*(strlen(pageDir))];
  sprintf(file, "/%d", id);
  strcat(pathName, file);
  if(page != NULL && pageDir != NULL && id > 0){
    FILE *fp = fopen(pathName, "w");
    if(fp != NULL){
      fprintf(fp, "%s\n %d\n %s\n", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
      count_free(pathName);
      fclose(fp);
      return true;
    }

  }
  count_free(pathName);
  // count_free(file);
  return false;

}

webpage_t* webpage_recreate(FILE *fp){
  char *url = freadlinep(fp);
  char *depth = freadlinep(fp);
  char *html = freadfilep(fp);
  //convert string number into integer
  int theDepth;
  if(sscanf(depth, "%d", &theDepth) != 1){
    fprintf(stderr, "Depth of this file was not found\n");
    return NULL;
  }
  webpage_t *page = webpage_new(url, theDepth, html);
  // count_free(url);
  count_free(depth);
  return page;

}

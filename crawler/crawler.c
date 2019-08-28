/* Ashley Song, CS50, 19X
*  Lab 4, Web crawler, 
*
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "webpage.h"
#include "memory.h"
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"

void crawler(char *urlSeed, char *pageDir, int maxDepth);
bool webpage_save(webpage_t *page, char *pageDir, int id);
void pageDelete(void *page);
void hashDelete(void *item);
inline static void logr(const char *word, const int depth, const char *url);

int main(const int argc, char *argv[]){
  char* program = argv[0];

  // check that argument length is validate
  if(argc != 4){
    fprintf(stderr, "usage: %s seedURL pageDirectory maxDepth\n", program);
    exit(1);
  }

  //check that the url is valid
  char* url = argv[1];
  if (!IsInternalURL(url) || !NormalizeURL(url)) {
    fprintf(stderr, "%s is not a valid url\n", url);
    exit(2);
  }

  char *pageDir = argv[2];
  //check pageDirectory is existing
  if(isPageDir(&pageDir) == false){
    exit(3);
  }
  //check that the directory has writer permission
  if(hasWritePermission(pageDir) == false){
    exit(4);
  }

  //check maxDepth value
  int maxDepth;
  char other;
  //if value something other than 1, user inputed argument that was not integer
  if ((sscanf(argv[3], "%d%c", &maxDepth, &other) != 1)){
    fprintf(stderr, "usage: maxDepth must be an integer value\n");
    exit(5);
  }
  else{
    if(maxDepth > 10 || maxDepth < 0){
      fprintf(stderr, "maxDepth must be in range [0, 10]\n");
      exit(7);
    }
  }
  printf("All inputted parameters are valid\n");
  //pass to crawler
  crawler(url, pageDir, maxDepth);
  exit(0);
}



void crawler(char *urlSeed, char *pageDir, int maxDepth){
  int id = 1;
  char *result = NULL;
  char *seed = count_calloc(strlen(urlSeed)+1, sizeof(char));
  strcpy(seed, urlSeed);
  //initialize the data structures
  bag_t *urlList = bag_new();
  assert(urlList != NULL);
  hashtable_t *hashtb = hashtable_new(100);
  assert(hashtb != NULL);

  //initialize seed URL
  //add webpage to bag to crawl
  webpage_t *seedPage = webpage_new(seed, 0, NULL);
  bag_insert(urlList, seedPage);
  hashtable_insert(hashtb, seed, "");

  //while there are more webpages to crawl
  webpage_t *currPage = bag_extract(urlList);
  int pageDepth;
  while(currPage != NULL){  //while the bag is not empty
    assert(currPage != NULL);
    if(webpage_fetch(currPage) != true){  //fetch the html
      fprintf(stderr, "webpage_fetch has failed for this url\n");
    }
    else{
      logr("Fetched:", webpage_getDepth(currPage), webpage_getURL(currPage));
      // assertp(webpage_getHTML(curr), "HTML not fetched");
      assert(webpage_save(currPage, pageDir, id) == true);  //save the info into the directory

      if(webpage_getDepth(currPage) < maxDepth){  //if current depth is not larger than max
        int pos = 0;
        while((result = webpage_getNextURL(currPage, &pos)) != NULL){ //while there are more urls to explore
          logr("Scanning:", webpage_getDepth(currPage), webpage_getURL(currPage));
          if(NormalizeURL(result) != true){
            fprintf(stderr, "URL failed to be normalized\n");
          }
          if(IsInternalURL(result) != true){
            logr("IgnrExtn:", webpage_getDepth(currPage) + 1, result);
            count_free(result);
          }
          else{
            logr("Found:", webpage_getDepth(currPage) + 1, result);
            if(hashtable_insert(hashtb, result, "")){ //if this url hasn't been visited yet
              pageDepth = webpage_getDepth(currPage) + 1;
              webpage_t *newPage = webpage_new(result, pageDepth, NULL);
              bag_insert(urlList, newPage); //add to bag
              logr("Added:", pageDepth, result);

            }
            else{
              logr("IgnrDup:", webpage_getDepth(currPage) + 1, result);
              count_free(result);
            }
          }
        }
      }

    }
    webpage_delete(currPage); //webpage_t struct not needed anymore, free it
    currPage = bag_extract(urlList);
    id++;
  }
  bag_delete(urlList, pageDelete);
  hashtable_delete(hashtb, hashDelete);

}


void pageDelete(void *page){
  if(page != NULL){
    count_free(page);
  }
}

void hashDelete(void *item){
  if(item != NULL){
    // count_free(item);
  }
}

// log one word (1-9 chars) about a given url
inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}

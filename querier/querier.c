/*
Ashley Song, CS50, 19X, Lab 6 Querier

Querier, in the TSE, is responsible for taking a search query from standard input and parsing the string
to produce an output. The output presents the document ID that the words in the query are found in as well
as the score for that document. The score is the calculated from through intersection and union of words.
An intersection is calculated anytime there is a sequence of words that might or might not be connected by
"and". The score for the intersection is calculated by finding the common documents with the "and" sequence
words and taking the minimum count between each word for the doc. The union score, on the other hand, is the
sum of word counts found between "or" sequences. The final for each doc is the combination of the intersection
scores to produce the union scores. The querier will rank the documents by highest to lowest score and
provide the URL of the page in which the words from the query input are found. Querier uses the files created
by crawler and the index_t struct and the index file created by the indexer to produce final results.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include "memory.h"
#include "hashtable.h"
#include "counters.h"
#include "pagedir.h"
#include "index.h"
#include "word.h"
#include "file.h"


/***************Structures***********/
int sortIndex = 0;		//struct to hold the result counter and the temp counter for each word

struct twocts {
	counters_t *result;
	counters_t *another;
};

struct idScore {		//struct to hold the id and score for each final result during sorting
	int id;
	int score;
};


/******************Functions**********************/
void querierFunc(char* input, index_t *index, char* dirName);		//main query function
counters_t* runQuery(index_t *index, char *words[20], int numWords, int orCount);	//called by querierFunc, responsible for calculating score
void counters_intersect(counters_t* ct1, counters_t* ct2);	//called by runQuery, calculates intersect score
void counters_union(counters_t* ct1, counters_t* ct2);		//called by runQuery, calculates union score
struct idScore* sortQueryResults(counters_t *finalSum, struct idScore *array);	//starts sorting process of final scores
void printQueryResults(struct idScore *array, char* pageDir);


/*************Static Helper Functions**************/
static int tokenizeLine(char *input, char *wordArray[20]);		//split query input into array of pointers to words
static bool validateCleanQuery(char *words[20], int numWords);	//check that query input is valid (correct usage of "and", "or")
static void intersect_helper(void *arg, const int key, const int count);	//called by counters_intersect
static void union_helper(void *arg, const int key, const int count);
static void createSortArray(void *arg, const int docID, const int count);
static void sortArray(struct idScore *array);
static char* getURL(int docID, char *dirName);
static void hashDelete(void *item);		//for cleaning up at end

//for testing, uncomment when using
// static void THEitemcount(void *arg, const int key, const int count);

static inline int min(const int a, const int b) {		//called by intersect_helper to find min between two values
  return (a < b ? a : b);
}


/***************Main******************/
int main(int argc, char *argv[]){
  char *program = argv[0];
  char *pageDir = argv[1];
  char *indexFile = argv[2];



  if(argc != 3){
    fprintf(stderr, "usage: <%s> <directory pathname> <index filename>\n", program);
    exit(1);
  }

  //first validate the input: pageDirectory and the indexFilename
  //does pageDir exist?
  if((isPageDir(&pageDir)) == false){
    exit(2);
  }

  //does pageDir have read permissions?
  if(hasReadPermission(pageDir) == false){
    exit(3);
  }
  //is this a crawler produced directory?
  if(isCrawlerDir(pageDir) == false){
    // exit(4);
  }

  //does indexFile exist?
  if((isFile(indexFile)) == false){
    exit(5);
  }
  //does indexFile have read permissions?
  if(hasReadPermission(indexFile) == false){
    exit(6);
  }

  //take in stdin until EOF
	index_t *index = indexLoad(indexFile);
  char *queryLine;
  while((queryLine = freadlinep(stdin)) != NULL){ //need to free queryLine at some point
    querierFunc(queryLine, index, pageDir);
    count_free(queryLine);
  }

  index_delete(index, hashDelete);
  exit(0);
}


/******************Query Functions********************/
void querierFunc(char* input, index_t *index, char* dirName){
   //assuming line will not exceed 20 words
  char*wordArray[20];
  int numWords = 0;
  int orCount = 0;
  bool validate;
	int i;


  for(i = 0; i < strlen(input); i++){   //check for bad characters
    if((isalpha(input[i]) == 0) && (isspace(input[i]) == 0)){
      fprintf(stderr, "Bad Character '%c' in query, moving onto next query\n", input[i]);
      return;
    }
  }

  if((numWords = tokenizeLine(input, wordArray)) == -1){
      fprintf(stderr, "Failed to create query array, moving onto next\n");
  }
  else{
    printf("Clean query: ");
    for(int i = 0; i < numWords; i++){
      normalizeWord(wordArray[i]);    //normalize each word in the array
      printf("%s ", wordArray[i]);
      if(strcmp(wordArray[i], "or")){
        orCount++;
      }
    }
		printf("\n");
    //check that query is valid
    validate = validateCleanQuery(wordArray, numWords);
    if(validate){
      counters_t *finalScore = runQuery(index, wordArray, numWords, orCount);
			struct idScore docScores[20];	//assume that the number of results is less than 20
			struct idScore *sortedScores = sortQueryResults(finalScore, docScores);
			printQueryResults(sortedScores, dirName);
			sortIndex = 0;
			counters_delete(finalScore);
    }
  }
}


counters_t* runQuery(index_t *index, char *words[20], int numWords, int orCount){
  //intersect until "or"
  //for AND, take minimum count between the two
  //for OR, take summation of counts for both words
	int i = 0;
	counters_t *currWord ;
	counters_t *interResults = NULL;
	counters_t *unionResults = counters_new();

	if(numWords < 2){
		currWord = index_find(index, words[0]);
		if(currWord != NULL){
			counters_union(unionResults, currWord);
		}
		else{
			fprintf(stderr, "No documents match.\n");
		}
		return unionResults;
	}
	while(i < numWords){
		if(strcmp(words[i], "or") == 0 || (i == numWords -1) ){
			if(interResults == NULL){
				currWord = index_find(index, words[i]);
				counters_union(unionResults, currWord);		//if last sequence is only one word
			}
			else if((i == numWords - 1) && interResults != NULL){
				currWord = index_find(index, words[i]);
				counters_intersect(interResults, currWord);
				counters_union(unionResults, interResults);	//if last "and" sequence and more than one word
			}
			else{
				counters_union(unionResults, interResults);		//create union score
				counters_delete(interResults);		//free this counter, reset for next round of "and" statements
				interResults = NULL;		//reset for new intersection score
			}
			i++;
		}
		else if(strcmp(words[i], "and") != 0 ){
			currWord = index_find(index, words[i]);
			if((interResults == NULL) && (currWord != NULL)){
				interResults = counters_new();
				counters_union(interResults, currWord);
				currWord = NULL;
			}
			else if((interResults != NULL) && (currWord != NULL)){
				counters_intersect(interResults, currWord);
				currWord = NULL;
				i++;
			}
			else{
				fprintf(stderr, "No documents match.\n");
				counters_delete(interResults);		//delete the results of intersection up until now
				interResults = NULL;
				currWord = NULL;
				while((i < numWords) && (strcmp(words[i], "or") != 0) ){		//if word is not found, increment i until you get to next "or" sequence
					i++;
				}
				i++; 	//skip past the "or" and start intersecting words after that
			}
		}
		else{
			i++;
		}
	}

	return unionResults;

	// counters_iterate(unionResults, 0, THEitemcount);	//for testing union function
}

struct idScore* sortQueryResults(counters_t *finalSum, struct idScore *array){
	struct idScore *docScores = array;
	counters_iterate(finalSum, docScores, createSortArray);	//array for the scores and docID
	sortArray(docScores);
	return docScores;
}

void printQueryResults(struct idScore *array, char* pageDir){
	char *url;
	printf("Matches %d documents(ranked):\n", sortIndex);
	for(int i =0; i < sortIndex; i++){
		url = getURL(array[i].id, pageDir);
		printf("Score		%d  Doc		%d: %s\n", array[i].score, array[i].id, url);
		count_free(url);
	}
}

/********************Helper Functions: Parsing Query Input***************************/
static int tokenizeLine(char *input, char *words[20]){
  int i = 0;
  char *p = strtok(input, " \t");
  int wordCount = 0;
  while(p != NULL){
    words[i++] = p;
    p = strtok(NULL, " \t");
    wordCount++;
  }
  if(wordCount == 0){
    fprintf(stderr, "Query cannot take only whitespace\n");
    return -1;
  }
  return wordCount;
}

static bool validateCleanQuery(char *words[20], int numWords){
  int i = 0;
  int j = 1;
  if(strcmp(words[0], "and") == 0 || strcmp(words[0],"or") == 0){
    fprintf(stderr, "'%s' cannot be first\n", words[0]);
    return false;
  }
  if(strcmp(words[numWords-1], "and") == 0 || strcmp(words[numWords-1],"or") == 0){
    fprintf(stderr, "'%s' cannot be last\n", words[numWords-1]);
    return false;
  }
  while(i < j && j < numWords){
    if((strcmp(words[i], "and") == 0 || strcmp(words[i],"or") == 0) && (strcmp(words[j], "and") == 0 || strcmp(words[j],"or") == 0)){
      fprintf(stderr, "'%s' and '%s' cannot be adjacent\n", words[i], words[j]);
      return false;
    }
    i++;
    j++;
  }
  return true;
}

/********************Helper Functions: Intersection and Union**************************/
void counters_union(counters_t* ct1, counters_t* ct2)
{
	assertp(ct1, "counters 1 invalid, union");
	assertp(ct2, "counters 2 invalid, union");

	struct twocts args = {ct1, ct2};
	counters_iterate(ct2, &args, union_helper);		//iterate the intersect counter
	// counters_delete(ct2);
	// ct2 = counters_new();		//reset after each union
}

static void union_helper(void *arg, const int key, const int count)
{
	struct twocts *two = arg;

	int sum = count + counters_get(two->result, key);

	counters_set(two->result, key, sum);
}


/*Taken from Lecture Activity */
void counters_intersect(counters_t* ct1, counters_t* ct2)
{
	assertp(ct1, "counters 1 invalid, intersect");
	assertp(ct2, "counters 2 invalid, intersect");

	struct twocts args = {ct1, ct2};
	counters_iterate(ct1, &args, intersect_helper);
}

static void intersect_helper(void *arg, const int key, const int count)
{
	struct twocts *two = arg;
	//
	// if(counters_get(two->another, key) == 0){
	// 	return;
	// }

	counters_set(two->result, key, min(count, counters_get(two->another, key)));
}


/*******************Helper Functions: Sorting Scores, Printing****************************/
static void createSortArray(void *arg, const int docID, const int count){
	struct idScore *array = arg;
	//keeping track of index with a global variable declared at the top.
	if((array != NULL) && (count > 0)){
		array[sortIndex].score = count;
		array[sortIndex].id = docID;
		sortIndex++;
	}
}

static void sortArray(struct idScore *array){
	int i;
	int j;
	struct idScore temp;

	//sorting the scores in descending order, bubble sort
  for (i = 0; i < sortIndex - 1; i++){
    for (j = 0; j < sortIndex - 1 - i; j++){
      if (array[j].score < array[j+1].score){
          temp = array[j];
          array[j] = array[j+1];
          array[j+1] = temp;
      }
    }
  }
}

static char* getURL(int docID, char *dirName){
	FILE *fp;
  char file[(strlen(dirName) + 5)];
  sprintf(file, "%s/%d", dirName, docID);
	fp = fopen(file, "r");
	char *url = freadlinep(fp);
	fclose(fp);
	return url;
}

/*****************Helper Functions: Cleaning Up********************/
static void hashDelete(void *item){    //as seen in crawler.c
  if(item != NULL){
    counters_delete(item);
  }
}

//for testing during code creation, uncomment during testing
// static void THEitemcount(void *arg, const int key, const int count){
//
//   printf("DocID: %d, Count: %d \n", key, count);
//
// }

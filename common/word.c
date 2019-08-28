/*
Ashley Song, CS50, 19X
Lab 5, indexer


word.c module, responsible for normalizing words.
Normalizing means to make every letter of a word a lower case.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*Takes each char of the word and turns it to lower case*/
void normalizeWord(char *result){

  for (int i = 0; i < strlen(result); i++){
    result[i] = tolower(result[i]);
  }
}

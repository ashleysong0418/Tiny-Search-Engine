/*
Ashley Song, CS50, 19X
Lab 5, indexer


Header File for the word.c module
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




/* normalizeWord is used to make sure all the characters in a word are lower case.
  Input: a char pointer to the word
  Ouput: nothing

  Using a for-loop to go through each character and using the toLower function

*/
void normalizeWord(char *result);

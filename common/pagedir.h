/*
* pagedir.h header file for pagedir module
* All functions that are related to accessing a directory.
*
*
* Ashley Song, CS50, 19X Lab4
*/

#include <stdio.h>
#include <stdbool.h>
#include "../libcs50/memory.h"
#include "../libcs50/webpage.h"

/* Functions Related to the directory*/
/*******************************************************/
/* isPageDir checks if the given page directory argument is an existing,
valid directory.

Input: a double pointer to the pageDir
Output: true if the directory is existing
        false if the directory is not existing or failed to open.

*/
bool isPageDir(char **pageDir);

/* isCrawlerDir checks to see if the given directory is made by the crawler module.
If given a directory name, this function tries to open a file withing the directory
called ".crawler". If the file is not able to open and is not readable, it is assumed
that the directory was not created by the crawler module.

Input: pointer to the directory name
Output: true if directory is crawler produced
        false if it not crawler produced
Note: this function only needs the directory name as it will concatenate the "/.crawler" path
to test if the directory is crawler produced.
*/
bool isCrawlerDir(char *dirCrawler);

/* isFile checks if the given file pathname is existing.
Input: a pointer to the file pathname
Output: true if the file exists
        false if the file does not exist
*/
bool isFile(char *fileName);

/* hasWritePermission checks if the given directory has the proper permissions
to be able to write into the directory.

Input: a pointer to the pageDir
Output: true if it has writer permission
        false if it does not have proper writer permission

Note: if directory does not have permission, change it using chmod
*/

bool hasWritePermission(char *pageDir);


/* hasReadPermission checks if the given directory has the proper permissions to be able to
read the contents of the directory.

Input: a pointer to the pageDir
Output: true if it has read permission
        false if it does not have proper read permissions

Note: if directory does not have read permission, change it using chmod
*/
bool hasReadPermission(char *pageDir);


/* webpage_save function creates and writes into a file in the given Directory.
  It writes in the URL, depth of the URL, and the HTML.

  Input: a webpage_t* struct, a pointer to the pageDir, and an integer id value
  Output: false if any of the inputs failed or were NULL
          true if the information was successfully saved into a file in the directory.

*/
bool webpage_save(webpage_t *page, char *pageDir, int id);

/* webpage_recreate is used to construc a webpage_t struct out of the information
given from a crawler directory file.

Input: a FILE *fp pointer to the file user wants to use for recreation
Output: a pointer to a webpage_t struct

*/
webpage_t* webpage_recreate(FILE *fp);

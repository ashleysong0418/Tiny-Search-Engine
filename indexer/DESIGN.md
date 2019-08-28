# TSE Indexer Design Spec
###### Ashley Song, CS50, 19X, lab5

The **indexer** takes the files created by the **crawler** and parses the file to collect data for storage in a data structure. From each file, **indexer** will gather the URL, depth, and the HTML to re-create a webpage struct for the purpose of parsing through the words in the document. **indexer**, through this process, will store the word count for each word in each document, later compiling the data and printing it to a file.


### User interface

The indexer's only interface with the user is on the command-line; it must always have two arguments.

```
./indexer <crawler directory file path name> <file name for the index output>
```

For example:

``` bash
$ ./indexer ../crawler/crawlertestdir indexFile
```

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: We save the contents of the `index_t` struct in a file.
The file will hold one word per line, followed by the doc id number and the word count for that doc. There can be multiple doc id/count pairs on the line for the word
Within a file, we write

 * <word> <doc id number> <word count for the doc>....
 *  <word> <doc id number> <word count for the doc>....
 .
 .
 .
 and so on.


### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *indexBuild*, which loops over files to parse, until all files in the directory have been parsed
 3. *webpage_recreate*, which extracts the URL, depth, and HTML from the file to create the webpage_t struct
 4. *wordCollect*, which loops over each word in the webpage and stores the word, doc ID, and the count to the index
 4. *normalizeWord*, which loops over each character in a word and converts it to the lower case
 5. *indexSave*, which takes the index data structure and iterates over it to print the contents into the file

And some helper modules that provide data structures:

 1. *index*, a wrapper struct for the hashtable data structure, specific for the indexer
 4. *counters*, the set-like data structure that is stored in the hashtable for the purpose of tracking the count for each word

### Pseudo code for logic/algorithmic flow

The indexer will run as follows:

1. validate the given arguments: check if directory exists and is readable, check if file has write permission or create the file if it doesn't already exist
2. initialize the data structures
3. build the index:
  4. for each crawler directory file,
    5. gather the file's info and create the webpage_t struct for that page
    6. collect the words in the page:
      7. for each word in the page, if the word is not less than 3:
        8. normalize the word
        9. if the word is in the index, increment the count
        10. if it is not in the index yet, add it to the index with a new counter
11. after all files have been parsed, save the index to the file according to the format described above
12. delete all initialized data structures


**normalize the word** means to convert each letter in the word to a lower case.
As one simple example,
`Home`
becomes
`home`.


For cleaner implementation, the code in this repository does not follow the exact pseudocode above. Rather, it breaks each of the large components into separate functions.

Notice that the output of the indexer is in a different order from the output of the following indextest. The order does not matter, and is due to the orderless structure of the index_t. However, to compare the files later, it is helpful to sort the file alphabetically and compare for differences. The correct results should have no differences.


The indexer completes and exits when it has no more files to be parsed.
We assume that the file names in the crawler directory are in numbered order, starting at 1 and incrementing by 1.


### Dataflow through modules

 1. *main* parses parameters and passes them to the crawler.
 2. *crawler* uses a bag to track pages to explore, and hashtable to track pages seen; when it explores a page it gives the page URL to the pagefetcher, then the result to pagesaver, then to the pagescanner.
 3. *pagefetcher* fetches the contents (HTML) for a page from a URL and returns.
 4. *pagesaver* outputs a page to the appropriate file.
 4. *pagescanner* extracts URLs from a page and returns one at a time.

### Major data structures

Three helper modules provide data structures:

 1. *set* of URLs (indirectly used by hashtable)
 2. *hashtable* of counters identified by key values set as words
 3. *counters* of doc ID and word count

### Testing plan

*Integration testing*.  Assemble the indexer and test it as a whole.
In each case, examine the output files carefully to be sure they have the correct words, doc IDs and counts.
Ensure that no words or counts are missing or duplicated.
Test with a shorter file for easier analysis.

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

0. Test the indexer with a non-existent directory.

0. Test the indexer with too few arguments.

0. Test the indexer with too many arguments.

1. Test the indexer with a short list of crawler files from small, closed off webpages.
Ensure that some words are repeated throughout each page and across pages.

2. Test the indexer with index test and compare the files after sorting alphabetically.

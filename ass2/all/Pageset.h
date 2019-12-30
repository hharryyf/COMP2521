#ifndef PAGESET_H
#define PAGESET_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "mylibrary.h"
#include "Hashing.h"

typedef struct urlpage {
	char **urls;  // set of urls
	int *id;   // set of ids
	int total_url; // total url in that rank file
} *Rank, Urlpage;

typedef struct set {
	int npages;
	Hash *maps;   // npages of maps
	Rank *pages;  // npages of urllist
	Rank ranknode;  // the ranknode page
	Hash mapnode;   // the newmap of the node
} *Pageset, Set;

// create a pageset withh pagenumber n
// along with n hashing maps which one for each page
Pageset newpageset(int n);
// init the pageset with a set of files
void initpageset(Pageset p, char **file, int file_num);
// print a rankfile
void printrank(Rank r);

// destroyrankfile

void destroyrank(Rank r);
// destroy the pageset
void destroypageset(Pageset p);

// print the pageset
void printpageset(Pageset p);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>
#include "mylibrary.h"
#include "Hashing.h"
#define MAX_LEN 1003

typedef struct page {
	float pagerank;
	int inset;
	int id;
	char *url;
} Page;

void get_total_url(int *total_url);

void destroy_collection (Page collection[], int total_url);

void search_pagerank(Hash h, Page collection[], int total_url, int argn, char *args[]);

int main (int argc, char *argv[]) {
	if (argc <= 1) {
		return 0;
	}
	
	FILE *fp;
	char line[MAX_LEN];
	char url[MAX_LEN];
	float rank;
	int id;
	int total_url = 0;
	int tmp;
	int MOD;
	
	Hash h = NULL;
	Page *collection = NULL;
	
	get_total_url(&total_url);
	
	if (total_url == 0) {
		exit(1);
	}
	
	if (total_url > MAX_LEN) {
		MOD = 1003;
	} else {
		MOD = total_url;
	}
	if (MOD == 0) {
		exit(1);
	}
	collection = calloc(total_url + 2, sizeof(Page));
	h = hash_init(MOD);
	
	fp = fopen("pagerankList.txt", "r");
	
	if (fp == NULL) {
		exit(1);
	}
	
	id = 0;
	while (fgets(line, MAX_LEN, fp) != NULL) {	
		if(sscanf(line, "%s %d, %f", url, &tmp, &rank) == 3) {
			url[strlen(url) - 1] = '\0';  // remove the ',' at the end
			hash_insert(h, hashkey(url, h), id, url);
			collection[id].pagerank = rank;
			collection[id].id = id;
			collection[id].url = mystrdup(url);
		    id++;	
		}	
	}
	fclose(fp);
	
    // hash_print(h);
	// find the words in the file 
	search_pagerank(h, collection, total_url, argc, argv);
	
	// destroy the memories
	destroy_collection(collection, total_url);
	hash_destroy(h);
	
	return 0;
}


int cmp(const void *pai, const void *paj) {
	Page *pa = (Page *) pai;
	Page *pb = (Page *) paj;
	if (pb->inset != pa->inset) {
		return pb->inset - pa->inset;
	} else {
		return pb->pagerank - pa->pagerank;
	}
}

// set is an array of Pages
void search_pagerank(Hash h, Page collection[], int total_url, int argc, char *argv[]) {
	int i, j = 0;
	char line[MAX_LEN];
	char tmp[MAX_LEN];
	FILE *fp;
	char *token;
	char delim[] = " \r\t\v\f\n";
	char **args = calloc(argc + 1, sizeof(char *));
	for (i = 0 ; i < argc - 1; i++) {
		args[i] = mystrdup(argv[i+1]);
	}
	
	/* for (i = 0 ; i < argc - 1; i++) {
		printf("%s\n", args[i]);
	} */
	
	for (i = 0 ; i < argc - 1; i++) {
		// argv[i] is the search string
		fp = fopen("invertedIndex.txt", "r");
		if (fp == NULL) {
			for (i = 0 ; i < argc - 1; i++) {
				free(args[i]);
			}
			free(args);
			exit(1);
		}
		// token the line
		while (fgets(line, MAX_LEN, fp) != NULL) {
			j = 0;
			for(token = strtok(line, delim); token != NULL; token = strtok(NULL, delim)) {  	
	        		if (j == 0 && strcmp(token, args[i]) != 0) {
	        			break;
	        		} 
	        	    // printf("%s ", token);
	        		strcpy(tmp, token);		
	        		if (hash_search(h, tmp) != -1) {  // if the search term is in the hashin table
	        			collection[hash_search(h, tmp)].inset += 1;  // increase the inset value
	        		}
	        		j++;
	        }
		}
		// grab the first token
		// if the first token match the argv[i]
		// then continue to get the urls from the token from the hash table
		// mark the collection[id].inset to be 1
		fclose(fp);
	}
	// free the args
	for (i = 0 ; i < argc - 1; i++) {
		free(args[i]);
	}
	free(args);
	
	qsort(collection, total_url, sizeof(Page), cmp);
	for (i = 0, j = 0 ; i < total_url && j < 30; i++) {
		if (collection[i].inset >= 1) {
			j++;
			printf("%s", collection[i].url);
			// printf(" %d", collection[i].inset);
			printf("\n");
		}
	}	
}

// destroy the collection struct array
void destroy_collection (Page collection[], int total_url) {
	int i;
	for (i = 0 ; i < total_url; i++) {
		free(collection[i].url);
	}
	free(collection);
}
// get the total number of url to decide how many url should be there
void get_total_url(int *total_url) {
	FILE *fp = fopen("pagerankList.txt", "r");
	char line[MAX_LEN];
	
	if (fp == NULL) {
		exit(1);
	}
	
	while (fgets(line, MAX_LEN, fp) != NULL) {	
		(*total_url)++;	
	}
	
	fclose(fp);
}


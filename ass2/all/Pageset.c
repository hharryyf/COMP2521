#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "Pageset.h"
#include "Hashing.h"
#include "mylibrary.h"
#define MAX_LEN 1003

// create a pageset withh pagenumber n
// along with n hashing maps which one for each page
Pageset newpageset(int n) {
	Pageset p = NULL;
	p = calloc(1, sizeof(Set));
	assert(p != NULL);
	p->npages = n;
	p->maps = calloc(n + 1, sizeof(Hash));
	p->pages = calloc(n + 1, sizeof(Rank));
	p->mapnode = NULL;
	p->ranknode = NULL;
	return p;
}
// init the pageset with a set of files
void initpageset(Pageset p, char **file, int filenum) {
	int i, j, sum = 0;
	char line[MAX_LEN];
	FILE *fp;
	for (i = 0 ; i < filenum - 1; i++) {
		fp = fopen(file[i + 1], "r");
		if (fp == NULL) {
			continue;
		}
		p->pages[i] = calloc(1, sizeof(Urlpage));
		p->pages[i]->total_url = 0;
		while (fscanf(fp, "%s", line) != EOF) {
			p->pages[i]->total_url += 1;
		}
		fclose(fp);
		
		fp = fopen(file[i+1], "r");
		
		if (fp == NULL) {
			continue;
		}
		p->pages[i]->urls = calloc(p->pages[i]->total_url + 1, sizeof(char *));
		p->pages[i]->id = calloc(p->pages[i]->total_url + 1, sizeof(int));
		if (p->pages[i]->total_url < 503) {
			p->maps[i] = hash_init(p->pages[i]->total_url);
		} else {
			p->maps[i] = hash_init(503);
		}
		j = 0;
		while (fscanf(fp, "%s", line) != EOF) {
			
			p->pages[i]->urls[j] = mystrdup(line);
			p->pages[i]->id[j] = j + 1;
			hash_insert(p->maps[i], hashkey(line, p->maps[i]), j+1, line);
			j++;
			sum++;
		}
		fclose(fp);	
	}
	
	p->mapnode = hash_init(sum > 503 ? 503 : sum);
	sum = 0;
	for (i = 0 ; i < p->npages; i++) {
		for (j = 0; j < p->pages[i]->total_url; j++) {
			if (hash_search(p->mapnode, p->pages[i]->urls[j]) == -1) {
				sum++;
				hash_insert(p->mapnode, hashkey(p->pages[i]->urls[j], p->mapnode), sum, p->pages[i]->urls[j]);
			}
		}
	}
	
	hash_destroy(p->mapnode);
	p->mapnode = hash_init(sum > 503 ? 503 : sum);
	p->ranknode = calloc(1, sizeof(Set));
	p->ranknode->total_url = sum;
	p->ranknode->urls = calloc(sum + 1, sizeof(char *));
	p->ranknode->id = calloc(sum + 1, sizeof(int));
	sum = 0;
	for (i = 0 ; i < p->npages; i++) {
		for (j = 0; j < p->pages[i]->total_url; j++) {
			if (hash_search(p->mapnode, p->pages[i]->urls[j]) == -1) {
				hash_insert(p->mapnode, hashkey(p->pages[i]->urls[j], p->mapnode), sum + 1, p->pages[i]->urls[j]);
				p->ranknode->urls[sum] = mystrdup(p->pages[i]->urls[j]);
				p->ranknode->id[sum] = sum + 1;
				sum++;
			}
		}
	}
	
}

// print a rankfile
void printrank(Rank r) {
	int i;
	if (r == NULL) {
		return;
	}
	for (i = 0 ; i < r->total_url; i++) {
		printf("%s %d\n", r->urls[i], r->id[i]);
	}
}

// destroyrankfile

void destroyrank(Rank r) {
	int i;
	if (r == NULL) {
		return;
	}
	for (i = 0 ; i < r->total_url; i++) {
		free(r->urls[i]);
		// printf("%s %d\n", r->urls, r->id);
	}
	free(r->urls);
	free(r->id);
	free(r);
}
// destroy the pageset
void destroypageset(Pageset p) {
	if (p == NULL) {
		return;
	}
	int i;
	for (i = 0 ; i < p->npages; i++) {
		destroyrank(p->pages[i]);
		hash_destroy(p->maps[i]);
	}
	free(p->pages);
	free(p->maps);
	destroyrank(p->ranknode);
	hash_destroy(p->mapnode);
	free(p);
} 

// print the pageset
void printpageset(Pageset p) {
	if (p == NULL) {
		return;
	}
	int i;
	for (i = 0 ; i < p->npages; i++) {
	    printf("%d ", p->pages[i]->total_url);
		printrank(p->pages[i]);
		printf("\n");
		hash_print(p->maps[i]);
		printf("\n");
	}
	printf("%d ", p->ranknode->total_url);
	printrank(p->ranknode);
	hash_print(p->mapnode);
}

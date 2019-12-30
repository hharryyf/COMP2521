#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mylibrary.h"
#include "AVL.h"
#include "Tfidf.h"
// cmp function used for qsort
int compare(const void *pai, const void *paj) {
	Page *pa = (Page *) pai;
	Page *pb = (Page *) paj;
	if (pa->match != pb->match) return pb->match > pa->match;
	return pb->tfidf > pa->tfidf;
}
// free the allocated memoryies
void destroy_page(Page page[], int total_url) {
	int i;
	for (i = 0 ; i < total_url; i++) {
		free(page[i].url);
	}
	free(page);
}

// times the term occur in the url / total words in the url
float tf(AVL t, char *term, char *url, Page page[], int total_url) {
	float tf = 0.0;
	int i, index = -1, frequency = 0;
	for (i = 0 ; i < total_url; i++) {
		if (page[i].url == NULL || url == NULL) {
			continue;
		}
		if (strcmp(page[i].url, url) == 0) {
			index = i;
			break;
		}
	}
	
	if (index == -1) {
		return 0.0;
	}
	
	if (!searchAVL(t, term)) {
		tf = 0.0;
	} else {
		frequency = searchurlAVL(t, term, url);
		if (frequency == 0) {
			return 0.0;
		} else {
			tf = (frequency * 1.0) / page[index].total_word;
		}
	}
	// printf("%s %d %d %f\n", term, frequency, page[index].total_word, tf);
	// printf("%s %s %d %f\n", term, url, total_url, tf);
	return tf;
}
// return log(the total_url / the number of document containing the term)
float idf(AVL t, char *term, char *url, Page page[], int total_url) {
	if (!searchreferenceAVL(t, term, url)) {
		return 0.0;
	} else {
		// printf("%s %d %d\n", term, total_url, searchreferenceAVL(t, term, url));
		return logf((total_url * 1.0) / (searchreferenceAVL(t, term, url) * 1.0)) / logf(10.0);
	}
}
// return the number of time the word occur in the url
int searchurlAVL(AVL t, char *term, char *url) {
	urlList *curr;
	if (t == NULL) {
		return 0;
	}
	
	if (strcmp(term, t->str) == 0) {
		curr = t->first;
		while (curr != NULL) {
			if (strcmp(curr->url, url) == 0) {
				return curr->time;
			}
			curr = curr->next;
		}
		// if it comes here means the url doesn't contain the term
		if (curr == NULL) {
			return 0;
		}
	}
	
	if (strcmp(t->str, term) < 0) {  // smaller then search right
		return searchurlAVL(t->right, term, url);
	} else {  // larger then search left
		return searchurlAVL(t->left, term, url);
	}
}
// return the url->size if not find return 0
int searchreferenceAVL(AVL t, char *term, char *url) {
	if (t == NULL) {
		// printf("we fail to search");
		return 0;
	}
	
	if (strcmp(term, t->str) == 0) {
		// printf("we found it %d\n", t->size);
		return t->size;
	}
	
	if (strcmp(t->str, term) < 0) {  // smaller then search right
		return searchreferenceAVL(t->right, term, url);
	} else {  // larger then search left
		return searchreferenceAVL(t->left, term, url);
	}
}
// all refers to the total url in collection.txt
void cal_tfidf(Page page[], int total_url, AVL t, char *args[], int total_term, int all) {
	int i, j;
	for (i = 1 ; i < total_term; i++) {
		for (j = 0 ; j < total_url; j++) {
			page[j].match += (searchurlAVL(t, args[i], page[j].url) > 0 ? 1 : 0);
			page[j].tfidf += tf(t, args[i], page[j].url, page, total_url)*idf(t, args[i], page[j].url, page, all);
		}
	}
	
	qsort(page, total_url, sizeof(Page), compare);
	
	for (i = 0 ; i < total_url && i < 30; i++) {
		 if (page[i].url != NULL && page[i].match != 0.0) {
		 		// printf("match = %d ", page[i].match);
				printf("%s %.6f\n", page[i].url, page[i].tfidf);
		}
	}
	
}

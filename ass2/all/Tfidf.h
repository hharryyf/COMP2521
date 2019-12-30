#ifndef TFidf_H
#define TFidf_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "AVL.h"

typedef struct page {
	char *url;        // url string
	int total_word;   // total word in the url
	int match;
	float tfidf;      //  tfidf value
} Page;

float tf(AVL t, char *term, char *url, Page page[], int total_url);

float idf(AVL t, char *term, char *url, Page page[], int total_url);

int searchurlAVL(AVL t, char *term, char *url);

int searchreferenceAVL(AVL t, char *term, char *url);

void destroy_page(Page page[], int total_url);

void cal_tfidf(Page page[], int total_url, AVL t, char *args[], int total_term, int all);

#endif

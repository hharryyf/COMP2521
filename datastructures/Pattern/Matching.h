#ifndef MATCH_H
#define MATCH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct node {
	struct node *next;
	int pos;
} *Match;

Match KMP(char str[], char ptr[]);

void destroy(Match l);

void showmatch(Match l);

int LongKMP(char str[], char ptr[]);

int BM(char str[], char ptr[]);
#endif

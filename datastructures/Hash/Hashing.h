#ifndef HASH_H
#define HASH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MOD 17389

typedef struct node {
	struct node *next;
	int value;
	char *name;
} Node;

typedef struct chain {
	Node **keys;
	int nkeys;
	int nchains;
} *Chain;

typedef struct Key {
	int value;
	char *name;
} KEY;

typedef struct doublehash {
	int nspots;
	int nkeys;
	KEY *keys;
} *DoubleHash;

Chain newchain();

char *chainSearch(Chain hc, int value, int *cmp);


void chainInsert(Chain hc, int value, char *name);

void destroyChain(Chain hc);

DoubleHash newhash();

void insertKey(DoubleHash h, int v, char *name);

char *searchKey(DoubleHash h, int v, int *cmp);

void dropdhash(DoubleHash h);
#endif

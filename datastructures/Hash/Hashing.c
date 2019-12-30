#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Hashing.h"
#define MOD 17389
#define MOD1 17
#define MOD2 11

static Node *newnode(int value, char *name);
static Node *insertInorder(Node *l, int value, char *name);

DoubleHash newhash() {
	DoubleHash t = malloc(sizeof(struct doublehash));
	t->nspots = MOD1;
	t->nkeys = 0;
	t->keys = calloc(MOD2 + 1, sizeof(KEY));
	int i;
	for (i = 0 ; i < MOD2 + 1; i++) {
		t->keys[i].value = -1;
		t->keys[i].name = NULL;
	}
	return t;
}

void insertKey(DoubleHash h, int v, char *name) {
	int spot = v % MOD1, inc = v % MOD2 + 1;
	if (h->nkeys >= h->nspots) return;
	int index = spot, count = 0;
	while (h->keys[index].value != -1) {
		if (h->keys[index].value == v) return;
		index = (index + inc) % h->nspots;
		count++;
		if (count == h->nspots) break;
	}
	h->keys[index].value = v;
	h->keys[index].name = strdup(name);
	h->nkeys++;
}

char *searchKey(DoubleHash h, int v, int *cmp) {
	int spot = v % MOD1, inc = v % MOD2 + 1;
	int index = spot, count = 0;
	while (h->keys[index].value != -1) {
		// printf("%d\n", index);
		(*cmp)++;
		count++;
		if (h->keys[index].value == v) return h->keys[index].name;
		index = (index + inc) % h->nspots;
		if (count == h->nspots) break;
	}
	return NULL;
}

void dropdhash(DoubleHash h) {
	free(h->keys);
	free(h);
}

Chain newchain() {
	Chain hc = calloc(1, sizeof(struct chain));
	hc->keys = calloc(MOD, sizeof(Node *));
	hc->nchains = MOD;
	hc->nkeys = 0;
	return hc;
}

char *chainSearch(Chain hc, int value, int *cmp) {
	Node *curr;
	assert(hc != NULL);
	assert(hc->nchains > 0 );
	curr = hc->keys[value % hc->nchains];
	while (curr != NULL) {
		(*cmp)++;
		if (curr->value == value) return curr->name;
		curr = curr->next;
	}
	return NULL;
}

static Node *newnode(int value, char *name) {
	Node *nd = malloc(sizeof(Node));
	nd->value = value;
	nd->name = strdup(name);
	nd->next = NULL;
	return nd;
}

static Node *insertInorder(Node *l, int value, char *name) {
	if (l == NULL) {
		l = newnode(value, name);
		return l;
	}
	
	Node *nd;
	if (l->value > value) {
		nd = newnode(value, name);
		nd->next = l;
		return nd;
	}
	l->next = insertInorder(l->next, value, name);
	return l;
}

void chainInsert(Chain hc, int value, char *name) {
	assert(hc != NULL);
	assert(hc->nchains > 0);
	int cmp = 0;
	if (chainSearch(hc, value, &cmp) != NULL) return;
	int spot = value % hc->nchains;
	hc->keys[spot] = insertInorder(hc->keys[spot], value, name); 
	hc->nkeys++;
}

static void destroychain(Node *l) {
	if (l != NULL) {
		destroychain(l->next);
		free(l);
	}
}

void destroyChain(Chain hc) {
	int i;
	for (i = 0 ; i < hc->nchains; i++) {
		destroychain(hc->keys[i]);
	}
	free(hc->keys);
	free(hc);
}

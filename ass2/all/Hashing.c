#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>
#include "mylibrary.h"
#include "Hashing.h"
#define MOD 127
// init a hashing table with total_key keys
Hash hash_init(int total_key) {
	Hash h = calloc(1, sizeof(struct hash));
	int i;
	h->total_key = total_key;
	h->key = calloc(total_key + 2, sizeof(Node *));
	for (i = 0 ; i < h->total_key; i++) {
		h->key[i] = NULL;
	}
	return h;
}
// return the key after mod N
int hashkey(char *url, Hash h) {
	int i, sum = 0;
	assert(h != NULL);
	if (url == NULL) {
		return -1;
	}
	// assert(h->total_key != 0);
	if (h->total_key == 0) {
		return -1;
	}
	
	if (strcmp(url, "") == 0) {
		return -1;
	}
	
	for (i = 0 ; i < strlen(url); i++) {
		sum = (sum * MOD + url[i]) % h->total_key;
	}
	
	sum = sum % h->total_key;
	
	return sum;
}
// insert a key
void hash_insert(Hash h, int keys, int id, char *url) {
	Node *new;
	if (keys == -1) {
		return;
	}
	
	if (hash_search(h, url) != -1) {
		return;
	}
	
	if (keys >= h->total_key) {
		keys = keys % h->total_key;
	}
	
	new = malloc(sizeof(Node));
	new->id = id;
	new->url = mystrdup(url);
	new->next = h->key[keys];
	h->key[keys] = new;
}
// search the key, if not find return -1 else return id
int hash_search(Hash h, char *url) {
	int i;
	Node *curr;
	assert(h != NULL);
	if (h->total_key == 0) {
		return -1;
	}
	
	i = hashkey(url, h);
	if (i == -1) {
		return -1;
	}
	
	curr = h->key[i];
	
	while (curr != NULL) {
		if (strcmp(curr->url, url) == 0) {
			return curr->id;
		}
		curr = curr->next;
	}
	return -1;
}

void hash_print(Hash h) {
	int i;
	Node *curr;
	for (i = 0 ; i < h->total_key; i++) {
		curr = h->key[i];
		while (curr != NULL) {
			printf("%s %d->", curr->url, curr->id);
			curr = curr->next;
		}
		printf("X\n");
	}
	
}

void hash_destroy(Hash h) {
	if (h == NULL) {
		return;
	}
	int i;
	for (i = 0 ; i < h->total_key; i++) {
		destroyhashList(h->key[i]);
	}
	
	free(h->key);
	free(h);
}

void destroyhashList(Node *l) {
	if (l == NULL) {
		return;
	}
	
	destroyhashList(l->next);
	free(l->url);
	free(l);
}

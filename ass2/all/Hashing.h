#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

typedef struct keys {
	struct keys *next;  // next pointer
	char *url;  // str of the url
	int id;  // id of the url
} Node;

typedef struct hash {
	Node **key; // an array of Node*
	int total_key; // total_keys
} *Hash;
// init a hashtable with n keys
Hash hash_init(int total_key);
// insert an element in the hash table
void hash_insert(Hash h, int keys, int id, char *url);
// seach in the hashtable and return the id of the url
int hash_search(Hash h, char *url);
// free up the memory of the hash table
void hash_destroy(Hash h);
// free up the list of a chain of a key 
void destroyhashList(Node *l);
// debugging print hash to see whether an element is in the correct spot
void hash_print(Hash h);
// return the key number of an element if it exists in the hash table
int hashkey(char *url, Hash h);
#endif

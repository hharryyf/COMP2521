#ifndef BIT_H
#define BIT_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct tree {
	int nkeys;
	int *item;
} *BIT, BITrep;

// create a newBIT
BIT newBIT(int n, int array[]);

// update the key by increment
void updatekey(BIT t, int index, int increment);

// get the sum of [from, to]
int intervalsum(BIT t, int from, int to);

// free the associated memory
void dropBIT(BIT t);

#endif

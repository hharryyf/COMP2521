#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "BinaryIndex.h"

static int getsum(BIT t, int upto);
static int getnext(int index);
static int getprev(int index);

BIT newBIT(int n, int array[]) {
	BIT t = malloc(sizeof(BITrep));
	t->nkeys = n;
	t->item = calloc(n + 2, sizeof(int));
	int i;
	// put the array of items into the BIT struct O(nlogn)
	for (i = 1; i <= n; i++) {
		updatekey(t, i, array[i]);
	}
	return t;
}

static int getnext(int index) {
	return index + (index & (-index));
}

static int getprev(int index) {
	return index - (index & (-index));
}


void updatekey(BIT t, int index, int increment) {
	if (index <= 0) return;
	if (index > t->nkeys) return;
	t->item[index] += increment;
	updatekey(t, getnext(index), increment);
}

static int getsum(BIT t, int upto) {
	if (upto <= 0) return 0;
	return t->item[upto] + getsum(t, getprev(upto));
}

int intervalsum(BIT t, int from, int to) {
	if (from < 0) from = 0;
	if (to > t->nkeys) to = t->nkeys;
	return getsum(t, to) - getsum(t, from - 1);
}

void dropBIT(BIT t) {
	assert(t != NULL);
	free(t->item);
	free(t);
}

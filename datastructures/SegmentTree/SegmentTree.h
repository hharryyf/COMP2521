#ifndef SEGT_H
#define SEGT_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define NOVALUE 99999999
typedef struct interval {
	int left;
	int right;
	int value;
} Interval;

typedef struct tree {
	Interval *item;
	int *lazy;
	int nkeys;  // number of keys in the tree
	int nitems; // total splts in the tree 2^n - 1
} *SegTree;

// return a new segtree correspond to an array of size n
SegTree newSegtree(int array[], int n);
// display the segment tree
void displaySegtree(SegTree t, int index, int depth);
// return the minimum value of an interval
int invervalmin(SegTree t, int start, int end);
// update value of a key indexed i to v in the segtree 
void updatekey(SegTree t, int index, int value);
// the intervel's min value after fixed interval
int lazy_invervalmin(SegTree t, int start, int end);
// lazy propagation
void lazy_propagation(SegTree t, int start, int end, int increment);

void dropsegtree(SegTree t);

#endif

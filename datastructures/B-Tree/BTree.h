#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
// the maximum child in the tree should be MAX_CHILD - 1
#define MAX_CHILD 5

// B tree struct
typedef struct btree {
	int value[MAX_CHILD];  // values in the node
	struct btree *child[MAX_CHILD];  // the child pointers
	int nnode;    //  1/2/3... means 1 or 2  or 3 ... values in a node // if this goes MAX_CHILD - 1 a split is needed
} *BTree, BTreerep;

// create a newroot node
BTree newroot(int value);
// search in the Btree
int searchBT(BTree t, int value);
// insertion in the BTree
BTree insertionBT(BTree t, int value);
// show the BTree (right slide)
void showBT(BTree t, int depth);
// drop the BTree
void dropBT(BTree t);
#endif

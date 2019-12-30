#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "BTree.h"

BTree newroot(int value) {
	BTree t = malloc(sizeof(BTreerep));
	int i;
	for (i = 0 ; i < MAX_CHILD; i++) {
		t->child[i] = NULL;
	}
	t->value[0] = value;
	t->nnode = 1;  // start from 1 node
	return t;
}

int searchBT(BTree t, int value) {
	int i;
	if (t == NULL) return 0;
	if (t->child[0] == NULL) {
		for (i = 0 ; i < t->nnode; i++) {
			if (t->value[i] == value) return 1;
		}
		return 0;
	}
	
	for (i = 0 ; i < t->nnode; i++) {
		if (t->value[i] == value) return 1;
	}
	
	if (t->value[0] > value) return searchBT(t->child[0], value);
	for (i = 1; i < t->nnode; i++) {
			if (value > t->value[i-1] && value < t->value[i]) return searchBT(t->child[i], value);
	}
	return searchBT(t->child[t->nnode], value);
}

static BTree insertBT(BTree t, int value) {
	int i, tmp, j;
	BTree nd1 = NULL;
	if (t == NULL) {
		t = newroot(value);
		return t;
	}
	
	// avoid duplicate values
	for (i = 0 ; i < t->nnode; i++) {
		if (t->value[i] == value) return t;
	}
	
	// found the root we insert it here
	if (t->child[0] == NULL) {
		t->value[t->nnode++] = value;
		for (i = t->nnode - 1; i > 0; i--) {
			if (t->value[i] < t->value[i-1]) {
				tmp = t->value[i];
				t->value[i] = t->value[i - 1];
				t->value[i - 1] = tmp;
			}
		}
		return t;
	}
	
	
	if (t->value[0] > value) {
		t->child[0] = insertBT(t->child[0], value);
	} else if (t->value[t->nnode - 1] < value) {
		t->child[t->nnode] = insertBT(t->child[t->nnode], value);
	} else {
		for (i = 1; i < t->nnode; i++) {
			if (value > t->value[i-1] && value < t->value[i]) {
				t->child[i] =  insertBT(t->child[i], value);
				break;
			}
		}
	}
	
	
	if (t->child[0] == NULL) return t;
	// showBT(t, 0);
	for (i = 0 ; i <= t->nnode; i++) {
		if (t->child[i]->nnode == MAX_CHILD - 1) {
			// we want to get the t->child[i]->value[1] to be upward
			t->value[t->nnode++] = t->child[i]->value[1];
			
			// swap so that the value is in the correct spot
			for (j = t->nnode - 1; j > 0; j--) {
				if (t->value[j] < t->value[j-1]) {
					tmp = t->value[j];
					t->value[j] = t->value[j - 1];
					t->value[j - 1] = tmp;
				}
			}
			
			
			// get the index of the promote value store in tmp
			for (j = t->nnode - 1; j >= 0; j--) {
				if (t->value[j] == t->child[i]->value[1]) {
					tmp = j;
					break;
				}
			}
		   
			
			// left is a 1-value-node
			nd1 = newroot(t->child[i]->value[0]);
			nd1->child[0] = t->child[i]->child[0];
			nd1->child[1] = t->child[i]->child[1];
			
			// right is a two-values-node
			t->child[i]->nnode = MAX_CHILD - 3;
			for (j = 0; j < MAX_CHILD - 3; j++) {
				t->child[i]->value[j] = t->child[i]->value[j + 2];
			}
			
			for (j = 0 ; j < MAX_CHILD - 2; j ++) {
				t->child[i]->child[j] = t->child[i]->child[j + 2];
			}
			t->child[i]->child[MAX_CHILD - 2] = NULL;
			t->child[i]->child[MAX_CHILD - 1] = NULL;
			
			
			for (j = t->nnode; j >= tmp + 2; j--) {
				t->child[j] = t->child[j - 1];
			}
			
			t->child[tmp + 1] = t->child[i];
			t->child[tmp] = nd1;
			
			break;
		}
	}
	return t;
}

BTree insertionBT(BTree t, int value) {
	if (searchBT(t, value)) return t;
	t = insertBT(t, value);
	BTree nd1, nd;
	int i;
	if (t->nnode == MAX_CHILD - 1) {
		 nd1 = newroot(t->value[0]);
		 nd1->child[0] = t->child[0];
		 nd1->child[1] = t->child[1];
		 nd = newroot(t->value[1]);
		 for (i = 0 ; i < MAX_CHILD - 3; i++) {
		 	t->value[i] = t->value[i + 2];
		 }
		 
		 for (i = 0 ; i < MAX_CHILD - 2; i++) {
		 	t->child[i] = t->child[i + 2];
		 }
		 nd->child[0] = nd1;
		 nd->child[1] = t;
		 t->nnode = MAX_CHILD - 3;
		 t = nd;
	}
	
	return t;
}

void dropBT(BTree t) {
	if (t == NULL) return;
	int i;
	for (i = t->nnode; i >= 0; i--) {
		dropBT(t->child[i]);
	}
	free(t);
}

void showBT(BTree t, int depth) {
	if (t == NULL) return;
	int i;
	for (i = t->nnode; i >= t->nnode / 2 + 1; i--) {
		showBT(t->child[i], depth + 1);
	}
	
	for (i = 0 ; i < depth; i++) {
		printf("\t\t\t");
	}
	
	printf("( ");
	for (i = 0 ; i < t->nnode; i++) {
		printf("%d ", t->value[i]);
	}
	printf(")\n");
	
	for (i = t->nnode / 2; i >= 0 ; i--) {
		showBT(t->child[i], depth + 1);
	}
}

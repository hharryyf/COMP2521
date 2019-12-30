#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

typedef struct tree {
	struct tree *left, *right;
	int value;
	int height;
} *AVL, AVLtree;

AVL newnode(int value);

int height(AVL t);

int searchAVL(AVL t, int value);

AVL insertAVL(AVL t, int value);

AVL deleteAVL(AVL t, int value);

AVL rebalanceAVL(AVL t);

AVL leftrotate(AVL t);

AVL rightrotate(AVL t);

void inorder(AVL t);

int cal_height(AVL t);

void showtree(AVL t, int depth);

void droptree(AVL t);
#endif

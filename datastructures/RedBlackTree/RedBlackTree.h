#ifndef RBT_H
#define RBT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define RED   -1
#define BLACK  1
#define LEFTLEFTTWO    1
#define LEFTRIGHTTWO   2
#define RIGHTLEFTTWO   3
#define RIGHTRIGHTTWO  4
#define LEFTLEFTONE    5
#define LEFTRIGHTONE   6
#define RIGHTLEFTONE   7
#define RIGHTRIGHTONE  8
#define BALANCE        0
typedef struct rbt {
	struct rbt *left, *right;
	int value;  
	int color;
} *RBT, RBTrep;

// newnode of a RBT
RBT newRBnode(int value);

// searchRBT same as BST standard search
int searchRBT(RBT t, int value);

// insertion in the RBT
RBT insertionRBT(RBT t, int value);
// show the RBT
void showRBT(RBT t, int depth);

// destroy the RBT
void destroyRBT(RBT t);
#endif

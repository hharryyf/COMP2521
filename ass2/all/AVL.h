#ifndef AVL_H
#define AVL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max(a, b) (a > b ? a : b)
#define TRUE 1
#define FALSE 0

typedef struct node {
	struct node *next;  // next node
	char *url;   // the url
	int time;   // times the word occur in the node
} urlList;

typedef struct tree {
	urlList *first; // first node in the url
	int size;       // sizeof the urllist
	urlList *last;	// last node in the url
	char *str; // the string
	int hi;   //  hight of the AVL tree
	struct tree *left;  // left node
	struct tree *right; // right node
} *AVL;

// create a AVL newnode
AVL newnode(char *str);

// search for a node in the AVL tree
int searchAVL(AVL t, char *str);

// inorder traversal of the AVL tree
void inorder(AVL t, FILE *out);

// normal tree left rotation
AVL leftRotate(AVL t);

// normal tree right rotation
AVL rightRotate(AVL t);

// insertion in the AVL tree
AVL insertAVL(AVL t, char *str, char *url);

// insert a url in the old AVL tree
AVL insertoldAVL(AVL t, char *str, char *url);

// insert a new node to the AVL tree
AVL insertnewAVL(AVL t, char *str, char *url);

// show the right-slide AVL tree for debugging
void showAVL(AVL t, int depth);

// free up the malloced memory of the AVL tree
void destroyAVL(AVL t);

// free up the urlList
void destroyList(urlList *l);

// delete operation in the AVL tree
AVL deleteAVL(AVL t, char *str);
// height of an AVL node
int height(AVL t);
// delete a node in the AVL tree
AVL deletenodeAVL(AVL t, char *str);
#endif

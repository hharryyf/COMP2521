#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node {
	struct node *next;
	int value;
} *List, Node;

List newnode(int value);

List duplicate(List l);

List removel(List l, int value);

List selectionSort(List l);

void droplist(List l);

void showlist(List l);
#endif

#ifndef PQ_H
#define PQ_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef int Item;

typedef struct heap {
	Item *item; // items represent as an array
	int nitems;  // nitems are now in the heap
	int nspots;  // max capasity of the heap
} Heaprep, *Heap;

Heap newHeap(int n);

int highpriority(Item v1, Item v2);

void popup(Heap h, int pos);

void insert(Heap h, Item v);

int isempty(Heap h);

void popdown(Heap h, int pos);

Item deleteheap(Heap h);

void showheap(Heap h, int pos, int depth);

void dropheap(Heap h);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "heap.h"

Heap newHeap(int n) {
	Heap h = calloc(1, sizeof(Heaprep));
	h->nspots = n;
	h->nitems = 0;
	h->item = calloc(n + 1, sizeof(Item));
	return h;
}

int highpriority(Item v1, Item v2) {
	return (v1 < v2 ? 1 : 0);
}

static void swap(Item *a, Item *b) {
	Item t;
	t = *a;
	*a = *b;
	*b = t;
}

void popup(Heap h, int pos) {
	if (pos <= 1) {
		return;
	}
	if (highpriority(h->item[pos], h->item[pos / 2])) {
		swap(&h->item[pos], &h->item[pos / 2]);
		popup(h, pos / 2);
	}
}

void insert(Heap h, Item v) {
	if (h->nitems >= h->nspots) {
		return;
	}
	
	h->item[++h->nitems] = v;
	popup(h, h->nitems); 
}

int isempty(Heap h) {
	return h->nitems >= 1 ? 0 : 1;
}

static int hasleftchild(Heap h, int pos) {
	return 2 * pos <= h->nitems ? 1 : 0;
}

static int hasrightchild(Heap h, int pos) {
	return 2 * pos + 1 <= h->nitems ? 1 : 0;
}

void popdown(Heap h, int pos) {
	if (pos > h->nitems) {
		return;
	}
	
	if (hasleftchild(h, pos) && hasrightchild(h, pos)) {
		if (highpriority(h->item[2*pos], h->item[2*pos+1])) {
			if (highpriority(h->item[2*pos], h->item[pos])) {
				swap(&h->item[2*pos], &h->item[pos]);
				popdown(h, pos*2);
			}
		} else {
			if (highpriority(h->item[2*pos+1], h->item[pos])) {
				swap(&h->item[2*pos+1], &h->item[pos]);
				popdown(h, pos*2+1);
			}
		}
	} else if (hasleftchild(h, pos)) {
		if (highpriority(h->item[2*pos], h->item[pos])) {
				swap(&h->item[2*pos], &h->item[pos]);
				popdown(h, pos*2);
		}
	} else if (hasrightchild(h, pos)) {
		if (highpriority(h->item[2*pos+1], h->item[pos])) {
				swap(&h->item[2*pos+1], &h->item[pos]);
				popdown(h, pos*2+1);
		}
	}
}

Item deleteheap(Heap h) {
	assert(!isempty(h));
	Item top = h->item[1];
	swap(&h->item[1], &h->item[h->nitems]);
	h->nitems--;
	popdown(h, 1);
	return top;
}

void showheap(Heap h, int pos, int depth) {
	if (pos > h->nitems) {
		return;
	}
	int i;
	showheap(h, pos*2+1, depth+1);
	for (i = 0 ; i < depth; i++) {
		printf("\t");
	}
	printf("%d\n", h->item[pos]);
	showheap(h, pos*2, depth+1);
}

void dropheap(Heap h) {
	free(h->item);
	free(h);
}

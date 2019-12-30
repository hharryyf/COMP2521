#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kruskal.h"
#include "GraphE.h"

int cmp(const void *pai, const void *paj) {
	Edge *pa = (Edge *) pai;
	Edge *pb = (Edge *) paj;
	return pa->weight > pb->weight;
}



int find(int f[], int x) {
	if (f[x] == x) {
		return x;
	} else {
		x = f[x];
		return find(f, x); 
	}
}

void union_set(int f[], int a, int b) {
	int x, y;
	x = find(f, a);
	y = find(f, b);
	f[x] = y;
}

Edge *Kruskal(Graph g) {
	int i, j;
	Edge *mst = calloc(g->nV + 1, sizeof(Edge));
	int f[g->nV];
	for (i = 0 ; i < g->nV; i++) {
		f[i] = i;
	}
	qsort(g->edges, g->nE, sizeof(Edge), cmp);
	for (i = 0, j = 0; i < g->nE && j < g->nV - 1; i++) {
		if (find(f, g->edges[i].start) != find(f, g->edges[i].end)) {
			mst[j++] = g->edges[i];
			union_set(f, g->edges[i].start, g->edges[i].end);
		}
	}
	
	return mst;
}

void print(Edge mst[], int size) {
	int i, total = 0;
	for (i = 0 ; i < size; i++) {
		printf("%d %d %d\n", mst[i].start, mst[i].end, mst[i].weight);
		total = total + mst[i].weight;
	}
	printf("total weight of the mst is %d\n", total);
}

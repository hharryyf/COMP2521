#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "GraphE.h"

Graph init(int nV, int nE) {
	Graph g = calloc(1, sizeof(struct graph));
	g->nE = nE;
	g->nV = nV;
	g->edges = calloc(g->nE + 1, sizeof(Edge));
	return g;
}

void addedge(Graph g, int index, int st, int ed, int w) {
	assert(g != NULL);
	assert(index < g->nE && index >= 0);
	g->edges[index].start = st;
	g->edges[index].end = ed;
	g->edges[index].weight = w;
}

void destroygraph(Graph g) {
	free(g->edges);
	free(g);
}

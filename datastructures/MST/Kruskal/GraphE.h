#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct edge {
	int start;
	int end;
	int weight;
} Edge;

typedef struct graph {
	Edge *edges;
	int nE;
	int nV;
} *Graph;

Graph init(int nV, int nE);
void addedge(Graph g, int index, int st, int ed, int w);
void destroygraph(Graph g);
#endif

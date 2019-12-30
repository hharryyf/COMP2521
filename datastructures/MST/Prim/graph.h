#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define INF 999999

typedef struct node {
	struct node *next;
	int vertex;
	int weight;
} Node;

typedef struct graph {
	Node **edges;
	int nV;
	int nE;
} Graphrep, *Graph;

Graph newgraph(int nV);

int isconnected(Graph g, int v1, int v2);

void addedge(Graph g, int v1, int v2, int w);

void Prim(Graph g, int src);

void dropgraph(Graph g);
#endif

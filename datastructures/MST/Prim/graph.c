#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

Graph newgraph(int nV) {
    // printf("hello");
	Graph g = calloc(1, sizeof(Graphrep));
	g->nE = 0;
	g->nV = nV;
	g->edges = malloc(g->nV * sizeof(Node *));
	int i;
	for (i = 0 ; i < g->nV; i++) {
		g->edges[i] = NULL;
	}
	// printf("hello");
	return g;
}

int isconnected(Graph g, int v1, int v2) {
	assert(g != NULL);
	Node *curr;
	if (g->nV > v1 && v1 >= 0 && v2 >= 0 && g->nV > v2) {
		curr = g->edges[v1];
		while (curr != NULL) {
			if (curr->vertex == v2) {
				return 1;
			}
			curr = curr->next;
		}
	}
	return 0;
}

void addedge(Graph g, int v1, int v2, int w) {
	Node *new;
	if (!isconnected(g, v1, v2)) {
		new = malloc(sizeof(struct node));
		new->vertex = v2;
		new->weight = w;
		new->next = g->edges[v1];
		g->edges[v1] = new;
		g->nE++;
	}
}


void Prim(Graph g, int src) {
	assert(g != NULL && src < g->nV && src >= 0);
	// printf("%d\n", g->nV);
	Heap pq = newHeap(1024);
	int prev[g->nV + 1], dist[g->nV + 1], visit[g->nV + 1];
	int i;
	Node *curr = g->edges[src];
	for (i = 0 ; i < g->nV; i++) {
		visit[i] = 0;
		dist[i] = INF;
		prev[i] = i;
	}
	Item it;
	it.vertex = src;
	it.weight = 0;
	insert(pq, it);
	while (curr != NULL) {
		dist[curr->vertex] = curr->weight;
		prev[curr->vertex] = src;
		it.vertex = curr->vertex;
		it.weight = curr->weight;
		insert(pq, it);
		curr = curr->next;
	}
	dist[src] = 0;
	visit[src] = 1;
	int index;
	Item min;
	// need to addvertex to set for another n-1 times
	for (i = 0 ; i < g->nV - 1; i++) {
		while (!isempty(pq)) {
			min = deleteheap(pq);
			if (!visit[min.vertex]) {
				// min = deleteheap(pq);
				break;
			}
		}
		visit[min.vertex] = 1;
		curr = g->edges[min.vertex];
		index = min.vertex;
		while (curr != NULL) {
			if (!visit[curr->vertex] && dist[curr->vertex] > curr->weight) {
				dist[curr->vertex] = curr->weight;
				prev[curr->vertex] = index;
				it.vertex = curr->vertex;
				it.weight = curr->weight;
				insert(pq, it);
			}
			curr = curr->next;
		}		
	}
	
	for (i = 0 ; i < g->nV; i++) {
		printf("%d %d\n", dist[i], prev[i]);
	}
	dropheap(pq);
}

static void droplist(Node *l) {
	if (l == NULL) {
		return;
	}
	
	droplist(l->next);
	free(l);
}

void dropgraph(Graph g) {
	if (g == NULL) {
		return;
	}
	
	int i;
	for (i = 0 ; i < g->nV; i++) {
		droplist(g->edges[i]);
	}
	free(g->edges);
	free(g);
}

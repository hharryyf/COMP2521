#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graph_h.h"
#include "Hashing.h"
#include "mylibrary.h"
Hash h = NULL;
// this function could be improved if the graph is implemented 
// by an adjMatrix
// however, no spec says that nV < 2000 such operation is unsafe
float Win(Graph g, int v, int u) {
	float ans = 0.0;
	float Iu = 0.0, Iv = 0.0;
	if (v >= g->nV || u >= g->nV || v < 0 || u < 0) {
		return 0.0;
	}
	// calculate the numorator
	// the number of in links to the point u
	Iu = g->in[u] * 1.0;
	// calculate the denomenator
	// iv is the sum of all the iu of the outlink of v
	Vertex *curr = g->vertexto[v];
	while (curr != NULL) {
		Iv = Iv + g->in[curr->id] * 1.0;
		curr = curr->next;
	}
	if (Iv == 0.0) {
		Iv = 0.5;
	}
	ans = Iu / Iv;
	return ans;
}
// this function could be improved if the graph is implemented 
// by an adjMatrix
// however, no spec says that nV < 2000 such operation is unsafe
float Wout(Graph g, int v, int u) {
	float ans = 0.0;
	float Ou = 0.0, Ov = 0.0;
	if (v >= g->nV || u >= g->nV || v < 0 || u < 0) {
		return 0.0;
	}
	// calculate the numorator
	// the number of in links to the point u
	Ou = g->out[u] * 1.0;
	if (g->out[u] == 0.0) {
		Ou = 0.5;
	}
	// calculate the denomenator
	// iv is the sum of all the iu of the outlink of v
	Vertex *curr = g->vertexto[v];
	while (curr != NULL) {
		Ov = Ov + g->out[curr->id] * 1.0;
		if (g->out[curr->id] == 0.0) {
			Ov = Ov + 0.5;
		}
		curr = curr->next;
	}
	if (Ov == 0.0) {
		Ov = 0.5;
	}
	ans = Ou / Ov;
	return ans;
}

Graph newGraph(int nV, char **urls) {
	Graph g = malloc(sizeof(Graphrep));
	int i;

	if (nV < 1004) {
		h = hash_init(nV);
	} else {
		h = hash_init(1003);
	}
	g->nV = nV;
	g->nE = 0;
	g->vertexto = malloc((g->nV + 1) * sizeof(Vertex *)); 
	g->vertexfrom = malloc((g->nV + 1) * sizeof(Vertex *)); 
	g->in = calloc((g->nV + 1), sizeof(int));
	g->out = calloc((g->nV + 1), sizeof(int));
    g->urlset = calloc((g->nV + 1), sizeof(char *));
	for (i = 0 ; i < g->nV; i++) {
		/*curr = malloc(sizeof(Vertex));
		curr->url = mystrdup(urls[i]);
		curr->id = i;
		curr->next = g->urlset;
		// g->urlset = curr;*/
		g->urlset[i] = mystrdup(urls[i]);
		g->vertexto[i] = NULL;
		g->vertexfrom[i] = NULL;
		hash_insert(h, hashkey(urls[i], h), i, urls[i]);
	}
	
	return g;
}
// check whether the two vertex are connected if so return 1 else return 0
// if one of te vertex is not in the graph return -1
int isconnected(Graph g, char *url1, char *url2) {
	int id1 = hash_search(h, url1), id2 = hash_search(h, url2);
	if (id1 == -1 || id2 == -1) {
		return -1;
	}
	Vertex *curr = g->vertexto[id1];
	while (curr != NULL) {
		if (curr->id == id2) {
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}
// add an Edge to the graph and the anti-graph
void addEdge(Graph g, char *url1, char *url2) {
	if (isconnected(g, url1, url2) == -1 || isconnected(g, url1, url2) == 1 || strcmp(url1, url2) == 0) {
		return;
	}
	int id1 = hash_search(h, url1), id2 = hash_search(h, url2);
	if (id1 == -1 || id2 == -1) {
		return;
	}
	Vertex *newnode1 = malloc(sizeof(Vertex)), *newnode2 = malloc(sizeof(Vertex));
	g->out[id1]++;// outlink ++
	g->in[id2]++;// inlink ++
	newnode1->id = id2;
	newnode1->url = mystrdup(url2);
	newnode1->next = g->vertexto[id1];
	g->vertexto[id1] = newnode1;
	newnode2->id = id1;
	newnode2->url = mystrdup(url1);
	newnode2->next = g->vertexfrom[id2];
	g->vertexfrom[id2] = newnode2;
	g->nE++;
}
// get the vertexurl of a graph
char *vertexurl(Graph g, int id) {
	assert(g != NULL);
	char *str = NULL;
	if (id < 0 || id >= g->nV) {
		return NULL;
	}
	str = mystrdup(g->urlset[id]);
	return str;
}
// show the graph this is a debug function
void showGraph(Graph g) {
	assert(g != NULL);
	int i;
	Vertex *curr;
	char *url = NULL;
	for (i = 0 ; i < g->nV; i++) {
		curr = g->vertexto[i];
		url = vertexurl(g, i);
		printf("%s (id = %d in = %d out = %d):\n", url, i, g->in[i], g->out[i]);
		free(url);
		printf("tovertex: ");
		while (curr != NULL) {
			printf("%s (%d)--> ", curr->url, curr->id);
			curr = curr->next;
		}
		printf("X\nfromvertex: ");
		curr = g->vertexfrom[i];
		while (curr != NULL) {
			printf("%s (%d)<-- ", curr->url, curr->id);
			curr = curr->next;
		}
		printf("X\n");
	}
}
// destroy the graph prevent memory leak
void destroyGraph(Graph g) {
	int i;
	for (i = 0 ; i < g->nV; i++) {
		removeList(g->vertexfrom[i]);
		removeList(g->vertexto[i]);	
		free(g->urlset[i]);
	}
	hash_destroy(h);
	free(g->urlset);
	free(g->vertexfrom);
	free(g->vertexto);
	free(g->in);
	free(g->out);
	free(g);
}
// remove a linked list
void removeList(Vertex *l) {
	if (l == NULL) {
		return;
	}
	removeList(l->next);
	free(l->url);
	free(l);
}
// get the vertexid using the Hashing.h
int vertexid(Graph g, char *url) {
	return hash_search(h, url);
}

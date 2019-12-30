#ifndef graph
#define graph

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertex {
	struct vertex *next;
	char *url;       // the url name
	int id;		  // id of the vertex 
} Vertex;

typedef struct page {
	int outdegree;
	char *url;
	float rank;
} Page;

typedef struct graph {
	int nV;
	int nE;    
	int *in;   // num of edges in to a vertex by id
	int *out;  // num of out edge from a vertex
	char **urlset;  // the urlset
	Vertex **vertexfrom;  // in vertex
	Vertex **vertexto;   // out vertex
} *Graph, Graphrep;

Graph newGraph(int nV, char **urls);

int isconnected(Graph g, char *url, char *url2);

void addEdge(Graph g, char *url1, char *url2);

char *vertexurl(Graph g, int id);

int vertexid(Graph g, char *url);

void showGraph(Graph g);

void removeList(Vertex *l);

void destroyGraph(Graph g);

float Win(Graph g, int v, int u);

float Wout(Graph g, int v, int u);
#endif

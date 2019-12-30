#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include "mylibrary.h"
#include "Hashing.h"
#include "graph_h.h"
#define MAX_LEN 1004
#define TRUE 1
#define FALSE 0

void destroy_urllist(char **urllist);

char **get_urllist(int nV);

int get_vertex();

void setup_connections(Graph g);

void cal_page_rank(Graph g, float d, float diffPR, int max_iteration);

int main (int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "supply the correct number of command line arguments\n");
		return 1;
	}
	// chdir("example1");
	char **urllist;
	int nV = 0;
	float d, diffPR;
	int max_iteration;
	Graph g = NULL;
	nV = get_vertex();
	// get the urllist from the file
	urllist = get_urllist(nV);
	// establish the vertex of the graph
	g = newGraph(nV, urllist);
	// set up the connection
	setup_connections(g);
	// calculate the pagerank
	sscanf(argv[1], "%f",&d);
	sscanf(argv[2], "%f", &diffPR);
	sscanf(argv[3], "%d", &max_iteration);
	/*
	int i, j;
	for (i = 0 ; i < g->nV; i++) {
		for (j = 0 ; j < g->nV; j++) {
			if (i != j)
			 	printf("Win[%d][%d]:%.7f, Wout[%d][%d]:%.7f\n", i, j, Win(g, i, j), i, j,Wout(g, i, j));		
		}
	}
	*/
	// calculate the pagerank
	cal_page_rank(g, d, diffPR, max_iteration);
	// free up the urllist 
    destroy_urllist(urllist);
    
    // printf("\n");
    // showGraph(g);
    // free up the graph
    destroyGraph(g);
	return 0;
}
// cmp function for qsort
int cmp(const void *pai, const void *paj) {
	Page *pa = (Page *) pai;
	Page *pb = (Page *) paj;
	return pb->rank > pa->rank;
}
// use *p[2] and mod 2 to save a lot of memory
void cal_page_rank(Graph g, float d, float diffPR, int max_iteration) {
	// printf("%f %f %d\n", d, diffPR, max_iteration);
	if (g == NULL) {
		return;
	}
	
	FILE *fp = fopen("pagerankList.txt", "w+");
	Page *p[2];
	int i = 0, j, k;
	Vertex *curr;
	float diff = diffPR;
	float old = 0.0;
	// set up the page rank
	p[0] = calloc(g->nV, sizeof(Page));
	p[1] = calloc(g->nV, sizeof(Page));
	for (i = 0 ; i < g->nV; i++) {
		p[0][i].outdegree = p[1][i].outdegree = g->out[i];
		p[0][i].url = vertexurl(g, i); // vertexurl(Graph g, int id)
		p[1][i].url = vertexurl(g, i);
		p[0][i].rank = 1.0/g->nV;
	}
	
	i = 0;
	while (i < max_iteration && diff >= diffPR) {	
		for (j = 0 ; j < g->nV; j++) {
			 old = 0.0;
			 curr = g->vertexfrom[j];
			 while (curr != NULL) {
			 	k = curr->id;
			 	old = old + Win(g, k, j)*Wout(g, k, j)*p[i%2][k].rank;
			 	curr = curr->next;
			 }
			 p[(i+1)%2][j].rank = old*d + (1.0 - d) /(g->nV); 
		}
		diff = 0.0;
		for (j = 0 ; j < g->nV; j++) {
			diff = diff + fabs(p[(i+1)%2][j].rank - p[i%2][j].rank);
		}
		i++;
	}
	// i % 2 is the newest pagerank
	qsort(p[i%2], g->nV, sizeof(Page), cmp);
	
	for (j = 0 ; j < g->nV; j++) {
		fprintf(fp, "%s, %d, %.7f\n", p[i%2][j].url, p[i%2][j].outdegree, p[i%2][j].rank);
	}
	
	for (j = 0 ; j < g->nV; j++) {
		free(p[0][j].url);
		free(p[1][j].url);
	}
	free(p[0]);
	free(p[1]);
	fclose(fp);
}
// set up the connections
void setup_connections(Graph g) {
	FILE *fp, *fp1;
	char file[MAX_LEN];
	char line[MAX_LEN];
	char text[MAX_LEN];
	char tmp[MAX_LEN];
	int find = FALSE;
	char *token;
	char delim[] = " \n\r\t\v\f";
	fp = fopen("collection.txt", "r");
	if (fp == NULL) {
		perror("fopen");
		exit(1);
	}
	while (fscanf(fp, "%s", line) != EOF) {
		strcpy(file, line);
		// get the .txt file
		strcat(file, ".txt");
		fp1 = fopen(file, "r");
		if (fp1 == NULL) {
			continue;
		}
		find = FALSE;
		/*while (fscanf(fp1, "%s", text) != EOF) {
			// end section 1 then stop
			if (strcmp(text, "#end") == 0) {
				break;
			}
			
			if (find == TRUE) {
				strcpy(tmp, text);
				// add an edge in the graph
	        	addEdge(g, line, tmp);  
			}
			// we read the first Section-1 then start addEdges
			if (strcmp(text, "Section-1") == 0) {
				find = TRUE;
			}
		}*/
		// read the urls in that section and add edges
		while (fgets(text, MAX_LEN, fp1) != NULL) {
			
			if (match_end_1(text)) {
				break;
			}
			
			if (find == TRUE) {
				for(token = strtok(text, delim); token != NULL; token = strtok(NULL, delim)) {  	
	        		strcpy(tmp, token);
	        		addEdge(g, line, tmp);
	        	}  
			}
			
			if (match_start_1(text)) {
				find = TRUE;
			}
		}
		fclose(fp1);
	}
	fclose(fp);
}
// this function will get the urllist
char **get_urllist(int nV) {
	FILE *fp;
	char tmp[MAX_LEN];
	int i;
	char **urllist = calloc(nV + 2, sizeof(char *));
	for (i = 0 ; i < nV + 1; i++) {
		urllist[i] = NULL;
	}
	
	fp = fopen("collection.txt", "r");
	if (fp == NULL) {
		perror("fopen");
		return NULL;
	}
	i = 0;
	// insert the urllist
	while (fscanf(fp, "%s", tmp) != EOF) {
		urllist[i] = mystrdup(tmp);
		i++;
	}
	fclose(fp);
	return urllist;
}
// get the number of vertex

int get_vertex() {
	int nV = 0;
	FILE *fp;
	char url[MAX_LEN];
	fp = fopen("collection.txt", "r");
	if (fp == NULL) {
		perror("fopen");
		return -1;
	}
	// it we found 1 url let nV++
	while (fscanf(fp, "%s", url) != EOF) {
		nV++;
	}
	
	fclose(fp);
	return nV;
}

void destroy_urllist(char **urllist) {
	int i = 0;
	while (urllist[i] != NULL) {
		free(urllist[i]);
		i++;
	}
	free(urllist);
}

#include <stdio.h>
#include <stdlib.h>
#include "kruskal.h"
#include "GraphE.h"

int main(int argc, char *argv[]) {
	FILE *fp = fopen("data.txt", "r");
	int nV, nE, st, ed, w;
	Graph g = NULL;
	fscanf(fp, "%d%d", &nV, &nE);
	g = init(nV, nE);
	int i;
	for (i = 0 ; i < nE; i++) {
		fscanf(fp, "%d%d%d", &st, &ed, &w);
		addedge(g, i, st, ed, w);
	}
	Edge *mst = Kruskal(g);
	fclose(fp);
	print(mst, g->nV - 1);
	free(mst);
	destroygraph(g);
	return 0;
}

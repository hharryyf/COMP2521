#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	char line[BUFSIZ];
	int n, v1, v2, w;
	Graph g = NULL;
	FILE *fp = fopen("data.txt", "r");
	fscanf(fp, "%d", &n);
	g = newgraph(n);
	// printf("hello\n");
	// printf("%d\n", g->nV);
	while (fgets(line, BUFSIZ, fp) != NULL) {
		if (sscanf(line, "%d %d %d", &v1, &v2, &w) == 3) {
			// printf("%d %d %d\n", v1, v2, w);
			addedge(g, v1, v2, w);
			addedge(g, v2, v1, w);
		}
	}
	fclose(fp);
	Prim(g, 1);
	dropgraph(g);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "heap.h"


int main (int argc, char *argv[]) {
	Heap h = newHeap(1024);
	Item v;
	char ch;
	FILE *fp = fopen("data.txt", "r");
	char line[BUFSIZ];
	while (fgets(line, BUFSIZ, fp) != NULL) {
		if (sscanf(line, "%c%d", &ch, &v) == 2) {
			if (ch == 'i') {	
				insert(h, v);
			} 
		} else if (ch == 'd') {
				printf("top of the heap is %d\n", deleteheap(h));
		}
	}
	
	showheap(h, 1, 0);
	dropheap(h);
	fclose(fp);
	return 0;
}



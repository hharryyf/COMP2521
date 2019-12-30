#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Matching.h"

int main(int argc, char *argv[]) {
	// char *m1 = "not";
	if (argc != 3) {
		fprintf(stderr, "supply the correct number of cmd args\n");
	}
	Match l = KMP(argv[1], argv[2]);
	printf("%d\n", LongKMP(argv[1], argv[2]));
	showmatch(l);
	destroy(l);
	printf("we use BM and find the the patterns are %s match\n", (BM(argv[1], argv[2]) != -1 ? "" : "not"));
	if (BM(argv[1], argv[2]) != -1) {
		printf("the first match ocurr when pos = %d\n", BM(argv[1], argv[2]));
	}
	return 0;
}

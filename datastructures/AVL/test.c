#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVL.h"

int main(int argc, char *argv[]) {
	AVL t = NULL;
	int i;
	while (scanf("%d", &i) != EOF) {
		t = insertAVL(t, i);
	}
	// showtree(t, 0);
	// while (t != NULL) t = deleteAVL(t, t->value);
	printf("the height of the tree is %d\n", cal_height(t));
	printf("the height of the tree is %d\n", height(t));
	droptree(t);
	return 0;
}

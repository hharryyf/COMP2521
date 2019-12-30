#include "BTree.h"

int main() {
	BTree t = NULL;
	int i;
	if (MAX_CHILD < 3) {
		printf("cannot establish the B-tree with order less than 3\n");
		return 1;
	}
	while (scanf("%d", &i) != EOF) {
		t = insertionBT(t, i);
	}
	showBT(t, 0);
	dropBT(t);
	return 0;
}

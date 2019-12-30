#include "SegmentTree.h"

int main(int argc, char *argv[]) {
	int i, n, st, ed;
	char ch;
	int *array;
	SegTree t = NULL;
	scanf("%d", &n);
	array = calloc(n + 1, sizeof(int));
	for (i = 0 ; i < n; i++) {
		scanf("%d", &array[i]);
	}
	
	t = newSegtree(array, n);
	while (scanf("%c %d %d", &ch, &st, &ed) != EOF) {
		if (ch == 'u') updatekey(t, st, ed);
		else if (ch == 'g') printf("%d\n", lazy_invervalmin(t, st, ed));
		else if (ch == 'i') {
			scanf("%d", &i);
			lazy_propagation(t, st, ed, i);
		}
	}
	// displaySegtree(t, 1, 0);
	
	dropsegtree(t);
	free(array);
	return 0;
}

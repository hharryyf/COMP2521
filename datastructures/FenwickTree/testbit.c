#include "BinaryIndex.h"

int main() {
	BIT t = NULL;
	int n, v, i;
	char ch;
	int *array;
	scanf("%d", &n);
	array = calloc(n + 1, sizeof(int));
	for (i = 1 ; i <= n; i++) {
		scanf("%d", &array[i]);
	}
	
	t = newBIT(n, array);
	
	while (scanf("%c%d%d", &ch, &v, &i) != EOF) {
		if (ch == 'u') {
			updatekey(t, v, i);
		} else if (ch == 's') {
			printf("the sum from %d to %d is %d\n", v, i, intervalsum(t, v, i));
		}
	}
	dropBIT(t);
	return 0;
}

#include "Radix.h"

int main(int argc, char *argv[]) {
	int *array = NULL;
	array = calloc(argc, sizeof(int));
	int i;
	for (i = 0 ; i < argc - 1; i++) {
		array[i] = atoi(argv[i + 1]);
	}
	RadixSort(array, argc - 1);
	for (i = 0 ; i < argc - 1; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
	free(array);
	return 0;
}

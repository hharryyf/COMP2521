#include <stdio.h>
#include <stdlib.h>

int cmp(const void *pai, const void *paj) {
	int *pa = (int *) pai;
	int *pb = (int *) paj;
	return (*pa) > (*pb);
}

int main(int argc, char *argv[]) {
	int *array = NULL;
	array = calloc(argc, sizeof(int));
	int i;
	for (i = 0 ; i < argc - 1; i++) {
		array[i] = atoi(argv[i + 1]);
	}
	qsort(array, argc - 1, sizeof(int), cmp);
	for (i = 0 ; i < argc - 1; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
	free(array);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "Sort.h"

int main(int argc, char *argv[]) {
	assert(argc >= 0);
	int array[argc], n = argc - 1;
	readin(array, argc, argv);
	MergeSortR(array, 0, n - 1);
	display(array, n);
	readin(array, argc, argv);
	MergeSortI(array,  n);
	display(array, n);
	readin(array, argc, argv);
	QuickSort(array,  0, n-1);
	display(array, n);
	readin(array, argc, argv);
	ShellSort(array,  n);
	display(array, n);
	return 0;
}

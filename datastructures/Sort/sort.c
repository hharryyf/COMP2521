#include <stdio.h>
#include <stdlib.h>
#include "Sort.h"

static void MergeArray(int array[], int low, int mid, int high);
static int Pivot(int array[], int low, int high);

void readin(int array[], int n, char *args[]) {
	int i;
	for (i = 0 ; i < n - 1; i++) {
		array[i] = atoi(args[i+1]);
	}
}

void MergeSortR(int array[], int low, int high) {
	if (low >= high) {
		return;
	}
	
	int mid = (low + high) / 2;
	MergeSortR(array, low, mid);
	MergeSortR(array, mid + 1, high);
	MergeArray(array, low, mid, high);
}

static void MergeArray(int array[], int low, int mid, int high) {
	int tmp[high + 1];
	int i, j, k;
	k = low;
	i = low;
	j = mid + 1;
	while (i <= mid && j <= high) {
		if (array[i] <= array[j]) {
			tmp[k] = array[i++];
			k++;
		} else {
			tmp[k] = array[j++];
			k++;
		}
	}
	
	while (i <= mid) {
		tmp[k] = array[i++];
		k++;
	}
	
	while (j <= high) {
		tmp[k] = array[j++];
		k++;
	}
	
	for (i = low; i <= high; i++) {
		array[i] = tmp[i];
	}
}

void MergeSortI(int array[], int n) {
	int low = 0, high = n - 1;
	int i, j;
	for (i = 1; i < n; i = i * 2) {
		for (j = 0 ; j < n - i; j = j + 2*i) {
			low = j;
			high = ((j + 2*i-1) > (n - 1) ? (n - 1) : (j + 2*i - 1));
			MergeArray(array, low, low + i - 1, high);
		}
	}
} 

void QuickSort(int array[], int low, int high) {
	if (low >= high) {
		return;
	}
	int key = Pivot(array, low, high);
	QuickSort(array, low, key - 1);
	QuickSort(array, key + 1, high);
}

static int findmedian(int array[], int low, int mid, int high) {
	if (array[low] <= array[mid] && array[mid] <= array[high]) {
		return mid;
	}
	
	if (array[high] <= array[mid] && array[mid] <= array[low]) {
		return mid;
	}
	
	if (array[low] <= array[high] && array[mid] >= array[high]) {
		return high;
	}
	
	if (array[low] >= array[high] && array[mid] <= array[high]) {
		return high;
	}
	
	return low;
}

static int Pivot(int array[], int low, int high) {
	int tmp, mid = (low + high) / 2;
	int index = findmedian(array, low, mid, high);
	tmp = array[low];
	array[low] = array[index];
	array[index] = tmp;
	int key = array[low];
	while (low < high) {
		while (low < high && array[high] >= key) {
			high--;
		}
		tmp = array[low];
		array[low] = array[high];
		array[high] = tmp;
		while (low < high && array[low] <= key) {
			low++;
		}
		tmp = array[low];
		array[low] = array[high];
		array[high] = tmp;
	}
	return low;
}

void ShellSort(int a[], int n) {
	int walk[6] = {4193, 1073, 281, 23, 8, 1}, step;
	int k = 0, i, tmp, j;
	for (k = 0; k < 6; k++) {
		step = walk[k];
		for (i = 0 ; i < n; i = i + step) {
			for (j = i; j >= step; j = j - step) {
				if (a[j] < a[j - step]) {
					tmp = a[j];
					a[j] = a[j - step];
					a[j - step] = tmp;
				}
			}
		}
	}
}

void display(int array[], int n) {
	int i;
	for (i = 0 ; i < n; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

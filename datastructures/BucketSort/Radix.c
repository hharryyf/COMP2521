#include "Radix.h"
#include "Queue.h"
#include <math.h>

Radix newRadix(int n) {
	Radix r = calloc(1, sizeof(Radixrep));
	r->nradix = n;
	r->bucket = calloc(n, sizeof(Queue));
	int i;
	for (i = 0 ; i < r->nradix; i++) {
		r->bucket[i] = newQueue();
	}
	return r;
}

void RadixSort(int *array, int size) {
	int i, j, max = -(1 << 30), div = 1, k;
	Radix r = newRadix(10);
	for (i = 0 ; i < size; i++) {
		if (array[i] > max) max = array[i];
	}
	
	for (i = 0 ; i <= log10(max); i++) {
		for (j = 0 ; j < size; j++) {
			enQueue(r->bucket[(array[j] / div) % 10], array[j]);
		}
		div = div * 10;
		k = 0;
		for (j = 0 ; j < 10; j++) {
			while (!isEmpty(r->bucket[j])) {
				array[k++] = deQueue(r->bucket[j]);
			}
		}
	}
	dropRadix(r);
}

void dropRadix(Radix r) {
	int i;
	for (i = 0 ; i < r->nradix; i++) dropQueue(r->bucket[i]);
	free(r->bucket);
	free(r);
}

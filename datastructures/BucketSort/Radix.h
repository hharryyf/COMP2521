#ifndef RADIX_H
#define RADIX_H

#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

typedef struct key {
	int nradix;
	Queue *bucket;
} *Radix, Radixrep;

Radix newRadix(int n);
void RadixSort(int *array, int size);
void dropRadix(Radix r);
#endif

#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
void MergeSortR(int array[], int low, int high);
void MergeSortI(int array[], int n);
void QuickSort(int array[], int low, int high);
void ShellSort(int array[], int n);
void display(int array[], int n);

#endif

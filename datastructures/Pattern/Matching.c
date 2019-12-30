#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Matching.h"

static Match newnode(int pos) {
	Match new = malloc(sizeof(struct node));
	new->next = NULL;
	new->pos = pos;
	return new;
}

static void cal_next(int next[], char ptr[]) {
	int i = 0, len = 0;
	next[i++] = 0;
	while (i < strlen(ptr)) {
		if (ptr[i] == ptr[len]) {
			next[i] = ++len;
			i++;
		} else {
			if (len == 0) {
				next[i++] = 0;	
			} else {
				len = next[len - 1];
			}
		}
	}
}

static void cal_last(int L[], char ptr[]) {
	int i, index;
	for (i = strlen(ptr) - 1; i >= 0; i--) {
		index = ptr[i];
		if (L[index] == -1) {
			L[index] = i;
		}
	}
}

int BM(char str[], char ptr[]) {
	// the last time of all the asciis occurr in the
	// pattern string
	int *L = calloc(128, sizeof(int));
	int i, j, index;
	for (i = 0 ; i < 128; i++) {
		L[i] = -1;
	}
	
	cal_last(L, ptr);
	
	i = strlen(ptr) - 1;
	j = strlen(ptr) - 1;
	
	while (i < strlen(str) && i >= 0) {
		j = strlen(ptr) - 1;
		while (j >= 0) {
			if (str[i] != ptr[j]) {
				index = str[i];
				if (L[index] + 1 < j) {
					i = i + strlen(ptr) - L[index] - 1;
				} else {
					i = i + strlen(ptr) - j;
				}
				break;
			}
			i--;
			j--;
		} 
		if (j == -1) {
			free(L);
			return i + 1;
		}
	}
	free(L);
	return -1;
}

Match KMP(char str[], char ptr[]) {
	assert(str != NULL && ptr != NULL);
	Match list = NULL, new = NULL;
	int *next = calloc(strlen(ptr) + 1, sizeof(int));
	int i = 0, j = 0;
	cal_next(next, ptr);
	while (i < strlen(str)) {
		
		if (ptr[j] == str[i]) {
			j++;
			i++;
		} else {
			if (j == 0) {
				i++;
			} else {
				j = next[j-1];
			}
		}
		
		if (j == strlen(ptr)) {
			new = newnode(i-j);
			new->next = list;
			list = new;
			// j = next[j-1]; match greedily
			j = 0;
		}
	}
	free(next);
	return list;
}

int LongKMP(char str[], char ptr[]) {
	assert(str != NULL && ptr != NULL);
	int *next = calloc(strlen(ptr) + 1, sizeof(int));
	int i = 0, j = 0, len = 0;
	cal_next(next, ptr);
	while (i < strlen(str)) {
		
		if (ptr[j] == str[i]) {
			j++;
			i++;
			if (len < j) {
				len = j;
			}
		} else {
			if (j == 0) {
				i++;
			} else {
				j = next[j-1];
			}
		}
		
		if (j == strlen(ptr)) {
			return j;
		}
	}
	free(next);
	return len;
}

void destroy(Match l) {
	if (l == NULL) {
		return;
	}
	destroy(l->next);
	free(l);
}

void showmatch(Match l) {
	if (l == NULL) {
		printf("X\n");
		return;
	}
	printf("%d->", l->pos);
	showmatch(l->next);
}



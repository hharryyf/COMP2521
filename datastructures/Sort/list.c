#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

List newnode(int value) {
	List new = malloc(sizeof(Node));
	new->value = value;
	new->next = NULL;
	return new;
}

List duplicate(List l) {
	List new;
	if (l == NULL) {
		return NULL;
	}
	new = newnode(l->value);
	new->next = duplicate(l->next);
	return new;
}

List removel(List l, int value) {
	if (l == NULL) {
		return NULL;
	}
	List tmp;
	if (l->value == value) {
		tmp = l;
		l = l->next;
		free(tmp);
	} else {
		l->next = removel(l->next, value);
	}
	return l;
}

List selectionSort(List l) {
	List new = duplicate(l);
	List curr, ans = NULL, newnd = NULL;
	int max = -999999;
	while (new != NULL) {
		max = new->value;
		curr = new;
		while (curr != NULL) {
			if (max < curr->value) {
				max = curr->value;
			}
			curr = curr->next;
		}
		new = removel(new, max);
		newnd = newnode(max);
		newnd->next = ans;
		ans = newnd;
	}
	return ans;
}

void droplist(List l) {
	if (l == NULL) {
		return;
	}
	droplist(l->next);
	free(l);
}

void showlist(List l) {
	List curr = l;
	while (curr != NULL) {
		printf("%d->", curr->value);
		curr = curr->next;
	}
	printf("X\n");
}

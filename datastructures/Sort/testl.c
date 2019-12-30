#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "supply the correct number of cmd line args\n");
		exit(1);
	}
	List l = NULL, tmp, ans = NULL;
	int i;
	for (i = 1 ; i < argc; i++) {
		tmp = newnode(atoi(argv[i]));
		tmp->next = l;
		l = tmp;
	}
	printf("the original list is \n");
	showlist(l);
	printf("the sorting list is \n");
	ans = selectionSort(l);
	showlist(ans);
	droplist(ans);
	droplist(l);
	return 0;
}

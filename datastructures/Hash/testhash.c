#include "Hashing.h"

int main(int argc, char *argv[]) {
	char ch;
	int value, cmp;
	char name[BUFSIZ];
	Chain hc = newchain();
	while (scanf("%c %d", &ch, &value) != EOF) {
		if (ch == 's') {
			cmp = 0;
			if (chainSearch(hc, value, &cmp) != NULL) {
				cmp = 0;
				printf("%d correcponds name is %s\n", value, chainSearch(hc, value, &cmp));
				printf("the number of comparison is %d\n", cmp);
			} else {
				printf("the value %d doesn't exit\n", value);
			}
		} else if (ch == 'i') {
			scanf("%s", name);
			chainInsert(hc, value, name);
		}
	}
	
	destroyChain(hc);
	return 0;
}

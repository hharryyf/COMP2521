#include "RedBlackTree.h"
int checkRBT(RBT t);
int checkRR(RBT t);
int checkBB(RBT t);
void preorder(RBT t, int step);
int judge = 1;
int num = 0;
int main(int argc, char *argv[]) {
	RBT t = NULL;
	int v;
	while (fscanf(stdin, "%d", &v) == 1) {
		t = insertionRBT(t, v);
	}
	
	// freopen("out.txt", "w", stdout);
    showRBT(t, 0);
    if (!checkRBT(t)) {
    	
    	printf("the tree isn't a valid RBT because\n");
    	if (t != NULL && t->color == RED) printf("the root of the tree is RED\n");
    	if (!checkRR(t)) printf("there is redred relationship\n");
    	if (!checkBB(t)) printf("there exits path that the number of black nodes are inequal\n");
    } else {
    	printf("the tree is a valid RBT with no RR relationship and every path the blacknode is %d\n", num);
    }
    destroyRBT(t);
	return 0;
}

int checkRR(RBT t) {
	if (t == NULL) return 1;
	if (t->left != NULL && t->color == RED && t->left->color == RED) return 0;
	if (t->right != NULL && t->color == RED && t->right->color == RED) return 0;
	return (checkRR(t->left) && checkRR(t->right));
}

void preorder(RBT t, int step) {
	if (t == NULL && num == 0) {
		num = step;
		return;
	}
	if (t == NULL) {
		judge = (judge && (step == num));
		return;
	}
	preorder(t->left, (t->color == RED ? step : step + 1));
	preorder(t->right, (t->color == RED ? step : step + 1));
}

int checkBB(RBT t) {
	judge = 1;
	num = 0;
	preorder(t, 0);
	return judge;
}

int checkRBT(RBT t) {
	if (t == NULL) return 1;
	if (t->color == RED) return 0;
	return (checkRR(t) && checkBB(t));
}

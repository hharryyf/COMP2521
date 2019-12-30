#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "RedBlackTree.h"

// tree right rotation
static RBT rightRotate(RBT t);

// tree left rotation
static RBT leftRotate(RBT t);
// recolor the RBT
static RBT recoloring(RBT t);
// return which kind of imbalance the RBT have
static int imbalance(RBT t);

// compare function
static int less(int v1, int v2) {
	return v1 < v2 ? 1 : 0;
}
// change the color of a node
static void decolor(RBT t) {
	if (t != NULL)  t->color = t->color * (-1);
}
// swapcolor of 2 nodes
static void swapcolor(RBT t1, RBT t2) {
	int color;
	if (t1 != NULL && t2 != NULL) {
		color = t1->color;
		t1->color = t2->color;
		t2->color = color;
	}
}
// newnode of a RBT
RBT newRBnode(int value) {
	RBT t = malloc(sizeof(RBTrep));
	t->left = NULL;
	t->right = NULL;
	t->value = value;  // red
	t->color = RED;
	return t;
}

// searchRBT same as BST standard search
int searchRBT(RBT t, int value) {
	if (t == NULL) return 0;
	if (t->value == value) return 1;
	if (less(value, t->value)) {
		return searchRBT(t->left, value);
	}
	return searchRBT(t->right, value);
}

// tree right rotation
static RBT rightRotate(RBT t) {
	if (t == NULL || t->left == NULL) return t;
	RBT tmp = t->left;
	t->left = tmp->right;
	tmp->right = t;
	return tmp;
}

// tree left rotation
static RBT leftRotate(RBT t) {
	if (t == NULL || t->right == NULL) return t;
	RBT tmp = t->right;
	t->right = tmp->left;
	tmp->left = t;
	return tmp;
}

static RBT rbinsert(RBT t, int value) {
	if (t == NULL) {
		t = newRBnode(value);
		return t;
	}
	
	if (t->value == value) return t;
	if (less(value, t->value)) {
		t->left = rbinsert(t->left, value);
	} else {
		t->right = rbinsert(t->right, value);
	}
	
	
	t = recoloring(t);
	
	return t;
}

static int imbalance(RBT t) {
	// no subtree cases return BALANCE
	if (t == NULL || (t->left == NULL && t->right == NULL)) return BALANCE;
	// only one child cases
	if (t->left == NULL) {
		if (t->right->left == NULL && t->right->right == NULL) return BALANCE;
		// t->right->right != NULL case
		if (t->right->left == NULL) {
			if (t->right->color == RED && t->right->right->color == RED) {
				return RIGHTRIGHTONE;
			}  // t->right->left != NULL case
		} else if (t->right->right == NULL) {
			if (t->right->color == RED && t->right->left->color == RED) {
				return RIGHTLEFTONE;
			}  // t->right->left != NULL case
		}
	} else if (t->right == NULL) {
		if (t->left->left == NULL && t->left->right == NULL) return BALANCE;
		// t->left->right != NULL case
		if (t->left->left == NULL) {
			if (t->left->color == RED && t->left->right->color == RED) {
				return LEFTRIGHTONE;
			}  // t->left->left != NULL case
		} else if (t->left->right == NULL) {
			if (t->left->color == RED && t->left->left->color == RED) {
				return LEFTLEFTONE;
			}  // t->right->left != NULL case
		}
	} else {
	// has two child cases
		if (t->left->color == RED && t->right->color == RED) {
			if (t->left->left != NULL && t->left->left->color == RED) {
				return LEFTLEFTTWO;
			} else if (t->left->right != NULL && t->left->right->color == RED) {
				return LEFTRIGHTTWO;
			} else if (t->right->left != NULL && t->right->left->color == RED) {
				return RIGHTLEFTTWO;
			} else if (t->right->right != NULL && t->right->right->color == RED) {
				return RIGHTRIGHTTWO;
			}
		} else if (t->left->color == RED){
			if (t->left->left != NULL && t->left->left->color == RED) {
				return LEFTLEFTONE;
			} else if (t->left->right != NULL && t->left->right->color == RED) {
				return LEFTRIGHTONE;
			}
		} else if (t->right->color == RED) {
			if (t->right->left != NULL && t->right->left->color == RED) {
				return RIGHTLEFTONE;
			} else if (t->right->right != NULL && t->right->right->color == RED) {
				return RIGHTRIGHTONE;
			}
		}
	}
	return BALANCE;
}
// recolor to make the RBT balance
static RBT recoloring(RBT t) {
	if (t == NULL) return t;
	
	if (imbalance(t) == LEFTLEFTTWO) {
		t->left->color = BLACK;
		t->right->color = BLACK;
		t->color = RED;
		// printf("we have case 1\n");
	} else if (imbalance(t) == LEFTRIGHTTWO) {
		t->left->color = BLACK;
		t->right->color = BLACK;
		t->color = RED;
		// printf("we have case 2\n");
	} else if (imbalance(t) == RIGHTLEFTTWO) {
		t->left->color = BLACK;
		t->right->color = BLACK;
		t->color = RED;
		// printf("we have case 3\n");
	} else if (imbalance(t) == RIGHTRIGHTTWO) {
		t->left->color = BLACK;
		t->right->color = BLACK;
		t->color = RED;
		// printf("we have case 4\n");
	} else if (imbalance(t) == LEFTLEFTONE) {
		swapcolor(t, t->left);
		t = rightRotate(t);
		// printf("we have case 5\n");
	} else if (imbalance(t) == LEFTRIGHTONE) {
		t->left = leftRotate(t->left);
		swapcolor(t, t->left);
		t = rightRotate(t);
		// printf("we have case 6\n");
	} else if (imbalance(t) == RIGHTLEFTONE) {
		t->right = rightRotate(t->right);
		swapcolor(t, t->right);
		t = leftRotate(t);
		// printf("we have case 7\n");
	} else if (imbalance(t) == RIGHTRIGHTONE) {
		swapcolor(t, t->right);
		t = leftRotate(t);
		// printf("we have case 8\n");
	}
	
	return t;
}
// show the RBT
void showRBT(RBT t, int depth) {
	if (t != NULL) {
		int i;
		showRBT(t->right, depth + 1);
		for (i = 0 ; i < depth; i++) printf("\t\t\t");
		/*if (t->color == RED) {
			 printf("\033[1;31m");
  			printf("%d\n", t->value);
  			printf("\033[0m;");
		} else {
			printf("%d\n", t->value);
		}*/
		printf("%d", t->value);
		if (t->color == RED) printf(" (red)\n");
		else printf(" (black)\n");
		showRBT(t->left, depth + 1);
	}
}
// insertion in the RBT public interface
RBT insertionRBT(RBT t, int value) {
	t = rbinsert(t, value);
	if (t->color == RED) {
		decolor(t);
	}
	return t;
}

// remove the RBT
void destroyRBT(RBT t) {
	if (t != NULL) {
		destroyRBT(t->left);
		destroyRBT(t->right);
		free(t);
	}
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVL.h"

AVL newnode(int value) {
	AVL new = malloc(sizeof(AVLtree));
	new->left = NULL;
	new->right = NULL;
	new->height = 0;
	new->value = value;
	return new;
}

int height(AVL t) {
	if (t == NULL) {
		return -1;
	}
	return t->height;
}

int cal_height(AVL t) {
	if (t == NULL) return -1;
	// int L = cal_height(t->left);
	// int R = cal_height(t->right);
    return (cal_height(t->left) > cal_height(t->right) ? cal_height(t->left) : cal_height(t->right)) + 1;
	// return 1 + (L > R ? L : R);
}

int searchAVL(AVL t, int value) {
	if (t == NULL) {
		return 0;
	}
	
	if (t->value == value) {
		return 1;
	}
	
	if (t->value > value) {
		return searchAVL(t->left, value);
	}
	return searchAVL(t->right, value);
}

static int minavl(AVL t) {
	assert(t != NULL);
	AVL curr = t;
	while (curr->left != NULL) {
		curr = curr->left;	
	}
	return curr->value;
}

AVL deleteAVL(AVL t, int value) {
	if (t == NULL) {
		return t;
	}
	AVL tmp;
	if (t->value == value) {
		if (t->left == NULL && t->right == NULL) {
			free(t);
			return NULL;
		} else if (t->left == NULL) {
			tmp = t;
			t = t->right;
			free(tmp);	
			return t;
		} else if (t->right == NULL) {
			tmp = t;
			t = t->left;
			free(tmp);
			return t;
		} else {
			t->value = minavl(t->right);
			t->right = deleteAVL(t->right, t->value);
		}
	}
	
	if (t->value > value) {
		t->left = deleteAVL(t->left, value);
	} else {
		t->right = deleteAVL(t->right, value);
	}
	
	t->height = 1 + (height(t->left) > height(t->right) ? height(t->left) : height(t->right));
	t = rebalanceAVL(t);
	t->height = 1 + (height(t->left) > height(t->right) ? height(t->left) : height(t->right));
	return t;
}

AVL insertAVL(AVL t, int value) {
	if (t == NULL) {
		t = newnode(value);
		return t;
	}
	
	if (t->value == value) {
		return t;
	}
	
	if (t->value < value) {
		t->right = insertAVL(t->right, value);
	} else {
		t->left = insertAVL(t->left, value);
	}
	
	t->height = 1 + (height(t->left) > height(t->right) ? height(t->left) : height(t->right));
	
	t = rebalanceAVL(t); 
	
	t->height = 1 + (height(t->left) > height(t->right) ? height(t->left) : height(t->right));
	return t;
}

AVL rebalanceAVL(AVL t) {
	if (t == NULL) {
		return t;
	}
	
	// no conflict
	if ((height(t->left) - height(t->right)) * (height(t->left) - height(t->right)) <= 1) {
		return t;
	}
	
	if (height(t->left) - height(t->right) > 1) {
		if (height(t->left->left) < height(t->left->right)) {
			t->left = leftrotate(t->left);
		}
		
		t = rightrotate(t);
		return t;
	}
	
	if (height(t->right) - height(t->left) > 1) {
		if (height(t->right->left) > height(t->right->right)) {
			t->right = rightrotate(t->right);
		}
		
		t = leftrotate(t);
	}
	return t;
}

AVL leftrotate(AVL t) {
	if (t == NULL) {
		return t;
	}
	
	if (t->right == NULL) {
		return t;
	}
	
	AVL tmp = t->right;
	t->height = 1 + (height(t->left) > height(tmp->left) ? height(t->left) : height(tmp->left));
	t->right = tmp->left;
	tmp->left = t;
	tmp->height = 1 + (height(tmp->left) > height(tmp->right) ? height(tmp->left) : height(tmp->right));
	return tmp;
}

AVL rightrotate(AVL t) {
	if (t == NULL) {
		return t;
	}
	
	if (t->left == NULL) {
		return t;
	}
	
	AVL tmp = t->left;
	t->height = 1 + (height(t->right) > height(tmp->right) ? height(t->right) : height(tmp->right));
	t->left = tmp->right;
	tmp->right = t;
	tmp->height = 1 + (height(tmp->right) > height(tmp->left) ? height(tmp->right) : height(tmp->left));
	return tmp;
}

void droptree(AVL t) {
	if (t == NULL) {
		return;
	}
	droptree(t->left);
	droptree(t->right);
	free(t);
}

void inorder(AVL t) {
	if (t == NULL) {
		return;
	}
	inorder(t->left);
	printf("%d ", t->value);
	inorder(t->right);
}

void showtree(AVL t, int depth) {
	if (t == NULL) {
		return;
	}
	
	int i;
	showtree(t->right, depth + 1);
	for (i = 0 ; i < depth; i++) {
		printf("\t\t");
	}
	printf("%d\n", t->value);
	showtree(t->left, depth + 1);
}

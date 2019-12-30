#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylibrary.h"
#include "AVL.h"

// this file is the implementation of the AVL 
// self balancing tree
// with and its idea is from the peudo code of the lecture slide
// however the deletion one is completely self-work

// search a string in the AVL tree
int searchAVL(AVL t, char *str) {
	if (t == NULL) {
		return FALSE;  // tree is NULL return FALSE
	}
	
	if (strcmp(t->str, str) == 0) {
		return TRUE;    // find the tree node return TRUE
	}
	
	if (strcmp(t->str, str) < 0) {  // smaller then search right
		return searchAVL(t->right, str);
	} else {  // larger then search left
		return searchAVL(t->left, str);
	}
}

// create a newnode containing the string
AVL newnode(char *str) {
	AVL t = malloc(sizeof(struct tree));
	t->hi = 0;  // hi = 0
	t->str = mystrdup(str);  // word is str
	t->left = NULL;
	t->right = NULL;
	t->size = 0;  // sizeof the url list is 0
	t->first = t->last = NULL;  // first last is NULL
	return t;
}
// insert in the AVL tree
AVL insertAVL(AVL t, char *str, char *url) {
	if (!searchAVL(t, str)) {  // if not in the AVL we need to insert
		t = insertnewAVL(t, str, url);
	} else {  // if in the AVL we then insert an url
		t = insertoldAVL(t, str, url);
	}
	// showAVL(t, 0);
    // printf("\n\n\n\n");
	return t;
}
// delete in the AVL tree
AVL deleteAVL(AVL t, char *str) {
	if (!searchAVL(t, str)) {  // if not in the AVL tree then simply return t
		return t;
	} else {  // if in the AVL tree then we delete the node
		t = deletenodeAVL(t, str);
		return t;
	}
}
// delete node function
AVL deletenodeAVL(AVL t, char *str) {
	AVL tmp, curr;
	urlList *p, *newnode;
	if (t == NULL) {
		return NULL;
	}
	// we find the node
	if (strcmp(t->str, str) == 0) {
		if (t->left == NULL && t->right == NULL) {
			destroyList(t->first);  // both subtree empty
			free(t->str);
			free(t);  // free it
			t = NULL;
			return t;
		} else if (t->left == NULL) {  // has right child not left child
			tmp = t;
			t = t->right;
			destroyList(tmp->first);
			free(tmp->str);
			free(tmp);
			return t;
		} else if (t->right == NULL) { // has left child not right child
			tmp = t;
			t = t->left;   // relink the left child
			destroyList(tmp->first);
			free(tmp->str);
			free(tmp);
			return t;
		} else {   // has both left and right child
			curr = t->right;
		    while (curr->left != NULL) {
				curr = curr->left;
			}
			destroyList(t->first);
			free(t->str);
			t->str = mystrdup(curr->str);
			t->first = t->last = NULL;
			p = curr->first;
			while (p != NULL) {
				newnode = malloc(sizeof(urlList));
				newnode->url = mystrdup(p->url);
				newnode->next = NULL;
				if (t->first == NULL) {
					t->first = t->last = newnode;
				} else {
					t->last->next = newnode;
					t->last = newnode;
				}
				p = p->next;
			}  // find the child which is smallest in the right subtree
			// copy the information
			// delete the that piece of information in the right subtree
			t->right = deletenodeAVL(t->right, curr->str);
			// add code here to deal with the rebalance
		}
		
	}
	// delete on the right hand side
	if (strcmp(t->str, str) < 0) {
		t->right = deletenodeAVL(t->right, str);
	} else {  // delete on the left tree
		t->left = deletenodeAVL(t->left, str);
		// add code here to deal with rebalance
		
	}
	// conflict with the AVL definition rebalance
	t->hi = max(height(t->left), height(t->right)) + 1;
	if (height(t->left) - height(t->right) > 1) {
		if (height(t->left->left) < height(t->left->right)) {
			t->left = leftRotate(t->left);
		}
		t = rightRotate(t);
	} else if (height(t->left) - height(t->right) < -1) {
		if (height(t->right->left) > height(t->right->right)) {
			t->right = rightRotate(t->right);
		}
		t = leftRotate(t);
	}
	t->hi = max(height(t->left), height(t->right)) + 1;
	return t;
}

AVL insertoldAVL(AVL t, char *str, char *url) {
	if (t == NULL) {
		return t;
	}
	int find = 0;
	urlList *curr, *tmp;
	if (strcmp(t->str, str) == 0) {
		// we find the str
		curr = malloc(sizeof(urlList));
		curr->url = mystrdup(url);
		curr->time = 1;
		curr->next = NULL;
		if (t->first == NULL) {
			// with no url is the word we simply add the url to the urllist
			t->first = t->last = curr;
			t->size++;
		} else {
		    if (strcmp(t->first->url, url) == 0 || strcmp(t->last->url, url) == 0) {
		    	// duplicated urls
		    	if (strcmp(t->first->url, url) == 0) {
		    		t->first->time++;  // add 1 to the url appearence time
		    	} else {
		    		t->last->time++;  // add 1 to the url appearence time
		    	}
		    	free(curr->url);  // no insertion needed free the memory
				free(curr);
		    } else if (strcmp(t->first->url, url) > 0) {
				curr->next = t->first;    // if the url is smaller than first url
				t->first = curr;
				t->size++;   // increment the size of urllist by 1
			} else if (strcmp(t->last->url, url) < 0) {
				t->last->next = curr;  // if the url is larger than the last url
				t->last = curr;     // increment the size by 1
				t->size++;
			} else {  // insersion in the middle case
				tmp = t->first;
				find = 0;  // see whether we found a duplicate
				while (tmp->next != NULL) {
					if (strcmp(tmp->url, url) == 0) {
						tmp->time++;   // if we found a duplicate we increment the time
						find = 1;
						break;
					}
					// if we found a spot for insertion we insert the url and break
					if (strcmp(tmp->url, url) < 0 && strcmp(tmp->next->url, url) > 0) {
						curr->next = tmp->next;
						t->size++;
						tmp->next = curr;
						break;
					}
					tmp = tmp->next;
				}
				// if we find 1 place with duplicate, we free the url
				if (find == 1) {
					free(curr->url);
					free(curr);	
				}
			}
		}
		 return t;
	}
	
	if (strcmp(t->str, str) > 0) {
		t->left = insertoldAVL(t->left, str, url);
	} else {
		t->right = insertoldAVL(t->right, str, url);
	}
	return t;
	
}

int height(AVL t) {
	if (t == NULL) {  // we let a NULL tree's height to be -1
		return -1;
	}
	return t->hi;  // otherwise return t->hi
	
}
// insert a newNode in the AVL tree
AVL insertnewAVL(AVL t, char *str, char *url) {
	urlList *curr;
	if (t == NULL) {
		t = newnode(str);
		curr = malloc(sizeof(urlList));
		curr->url = mystrdup(url);
		curr->time = 1;
		curr->next = NULL;
		t->first = t->last = curr;
		t->size++;
		return t;
	}
	
	// insert on the right subtree
	if (strcmp(t->str, str) > 0) {
		t->left = insertnewAVL(t->left, str, url);
		
	} else {
		t->right = insertnewAVL(t->right, str, url);
	}
	// if the balance is destroyed
	if (height(t->left) - height(t->right) > 1) {
		// if the trouble is caused by the right subtree of the left child
		if (strcmp(t->left->str, str) < 0) {
			t->left = leftRotate(t->left); // first rotation
		} 
		t = rightRotate(t);// second rotation
	} else if (height(t->left) - height(t->right) < -1) {
		if (strcmp(t->right->str, str) > 0) {
		// trouble caused by right-left
			t->right = rightRotate(t->right);
		} 
		t = leftRotate(t);
	}
	// renew the height of t
	t->hi = max(height(t->left), height(t->right)) + 1;	
	return t;
}
// right rotate with some fixing height
AVL rightRotate(AVL t) {
	if (t == NULL) {
		return t;
	}
	
	if (t->left == NULL) {
		return t;
	}
	
	t->hi = max(height(t->left->right), height(t->right)) + 1;
	AVL tmp = t->left;
	t->left = tmp->right;
	tmp->right = t;
	tmp->hi = max(height(tmp->left), height(tmp->right)) + 1;
	return tmp;
}
// left rotate
AVL leftRotate(AVL t) {
	if (t == NULL) {
		return t;
	}
	
	if (t->right == NULL) {
		return t;
	}
	t->hi =  max(height(t->right->left), height(t->left)) + 1;
	AVL tmp = t->right;
	t->right = tmp->left;
	tmp->left = t;
	tmp->hi =  max(height(tmp->left), height(tmp->right)) + 1;
	return tmp;
}
// inorder traversal to deal with the printing to the file
void inorder(AVL t, FILE *out) {
	if (t == NULL) {
		return;
	}
	
	urlList *curr;
	inorder(t->left, out);
	fprintf(out, "%s ", t->str);
	// fprintf(out, "%d", t->hi);
	curr = t->first;
	while (curr != NULL) {
		fprintf(out, "%s ", curr->url);
		curr = curr->next;
	}
	fprintf(out, "\n");
	inorder(t->right, out);
}

// borrow the idea from the showTree in the lecture slide
// for debugging the balance
void showAVL(AVL t, int depth) {
	if (t == NULL) {
		return;
	}
	int i;
	
	showAVL(t->right, depth + 1);
	for (i = 0 ; i < depth; i++) {
		printf("\t\t\t\t");
	}
	printf("%s (%d:%d): ", t->str, t->hi, t->size);
	urlList *curr;
	curr = t->first;
	while (curr != NULL) {
		printf("%s(%d)->", curr->url, curr->time);
		curr = curr->next;
	}
	printf("X\n");
	showAVL(t->left, depth + 1);
}
// destroy the AVL tree
void destroyAVL(AVL t) {
	if (t == NULL) {
		// free(t);
		return;
	}
	destroyAVL(t->left);
	destroyAVL(t->right);
	destroyList(t->first);
	// free(t->first);
	free(t->str);
	free(t);
}
// destroy the urlList
void destroyList(urlList *l) {
	if (l == NULL) {
		return;
	}
	destroyList(l->next);
	free(l->url);
	free(l);
}

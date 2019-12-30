#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define min(a, b)  (a > b ? b : a)
#define NOVALUE 1000000001

typedef struct interval {
	int left;
	int right;
	int value;
} Interval;

typedef struct tree {
	Interval *item;
	int *lazy;
	int nkeys;  // number of keys in the tree
	int nitems; // total splts in the tree 2^n - 1
} *SegTree;

// return a new segtree correspond to an array of size n
SegTree newSegtree(int array[], int n);
// display the segment tree
void displaySegtree(SegTree t, int index, int depth);
// return the minimum value of an interval
int invervalmin(SegTree t, int start, int end);
// update value of a key indexed i to v in the segtree 
void updatekey(SegTree t, int index, int value);
// the intervel's min value after fixed interval
int lazy_invervalmin(SegTree t, int start, int end);
// lazy propagation
void lazy_propagation(SegTree t, int start, int end, int increment);

void dropsegtree(SegTree t);

static int getsize(int n);
static void getkeys(SegTree t, int index, int left, int right, int array[]);
static void fixkey(SegTree t, int index, int currindex, int value);
static int getmin(SegTree t, int start, int end, int index);
static void propagate(SegTree t, int start, int end, int index, int increment);
static int lazy_getmin(SegTree t, int start, int end, int index);

int main() {
	int i, n, st, ed, m, v;
	int *array;
	SegTree t = NULL;
	scanf("%d%d", &n, &m);
	array = calloc(n + 1, sizeof(int));
	for (i = 0 ; i < n; i++) {
		scanf("%d", &array[i]);
	}
	
	t = newSegtree(array, n);
	
	// displaySegtree(t, 1, 0);
	for (i = 0 ; i < m; i++) {
		scanf("%d%d%d", &v, &st, &ed);
		
		if (lazy_invervalmin(t, st, ed) < v) {
			printf("-1\n%d\n", i + 1);
			// dropsegtree(t);
			// free(array);
			return 0;
		}
		lazy_propagation(t, st, ed, -v);
	}
	printf("0\n");
	// dropsegtree(t);
	// free(array);
	return 0;
}

static int getsize(int n) {
	int i = 1;
	while (i < n) {
		i = i * 2;
	}
	return i;
}

static void getkeys(SegTree t, int index, int left, int right, int array[]) {
	
	t->item[index].left = left;
	t->item[index].right = right;
	if (left > right || left <= 0 || right > t->nitems) return;
	if (left == right) {
		t->item[index].value = array[left - 1];
	} else {
		getkeys(t, index * 2, left, (left + right) / 2, array);
		getkeys(t, index * 2 + 1, (left + right) / 2 + 1, right, array);
		t->item[index].value = min(t->item[index * 2].value, t->item[index * 2 + 1].value);
	}
}
// return a new segtree correspond to an array of size n
SegTree newSegtree(int array[], int n) {
	SegTree t = calloc(1, sizeof(struct tree));
	int nitem = 2 * getsize(n) - 1, i;
	t->nitems = nitem;
	t->nkeys = n;
	t->item = calloc(nitem + 1, sizeof(Interval));
	t->lazy = calloc(nitem + 1, sizeof(int));
	for (i = 0 ; i <= t->nitems; i++) {
		t->item[i].left = NOVALUE;
		t->item[i].right = NOVALUE;
		t->item[i].value = NOVALUE;
		t->lazy[i] = 0;
	}
	
	getkeys(t, 1, 1, n, array);
	return t;
}

void displaySegtree(SegTree t, int index, int depth) {
	if (t == NULL) return;
	if (index > t->nitems) return;
	int i;
	displaySegtree(t, index * 2 + 1, depth + 1);
	for (i = 0 ; i < depth; i++) printf("\t\t");
	printf("%d (%d, %d)\n", t->item[index].value, t->item[index].left, t->item[index].right);
	displaySegtree(t, index * 2, depth + 1);
}

static int getmin(SegTree t, int start, int end, int index) {
	if (index > t->nitems || index <= 0) return NOVALUE;
	if (t->item[index].left >= start && end >= t->item[index].right) return t->item[index].value;
	if (t->item[index].left > end || t->item[index].right < start) return NOVALUE;
	return min(getmin(t, start, end, index * 2), getmin(t, start, end, index * 2 + 1));
}

// return the minimum value of an interval
int invervalmin(SegTree t, int start, int end) {
	assert(t->nkeys >= 1);
	assert(start <= end);
	int value = getmin(t, start, end, 1);
	return value;
}

// fix the keys
static void fixkey(SegTree t, int index, int currindex, int value) {
	if (t->item[currindex].left == t->item[currindex].right && t->item[currindex].left == index) {
		t->item[currindex].value = value;
		return;
	}
	if (t->item[currindex].left <= index && index <= (t->item[currindex].left + t->item[currindex].right) / 2) {
		fixkey(t, index, currindex * 2, value);
	} else {
		fixkey(t, index, currindex * 2 + 1, value);
	}
	t->item[currindex].value = min(t->item[currindex * 2].value, t->item[currindex * 2 + 1].value);
}

static int lazy_getmin(SegTree t, int start, int end, int index) {
	if (index > t->nitems || index <= 0) return NOVALUE;
	
	if (t->lazy[index] != 0) {
		t->item[index].value += t->lazy[index];
		if (t->item[index].left != t->item[index].right) {
			t->lazy[2 * index] += t->lazy[index];
			t->lazy[2 * index + 1] += t->lazy[index];
		}
		t->lazy[index] = 0;
	}
	
	// disjoint case simply return
	if (start > t->item[index].right || end < t->item[index].left) return NOVALUE;
	// total overlap case return the value plus the increment to the subtrees
	if (start <= t->item[index].left && end >= t->item[index].right) {
		return t->item[index].value;
	}
	return min(lazy_getmin(t, start, end, 2 * index), lazy_getmin(t, start, end, 2 * index + 1));
}

// use the lazy array from start to end
int lazy_invervalmin(SegTree t, int start, int end) {
	assert(t->nkeys >= 1);
	int value = lazy_getmin(t, start, end, 1);
	return value;
}

static void propagate(SegTree t, int start, int end, int index, int increment) {
	// exit condition
	if (t->item[index].left > t->item[index].right || t->item[index].left > t->nkeys) return;
	// there is some value in the lazy tree
	// move the lazy value down and reset the lazy value to zero
	if (t->lazy[index] != 0) {
		if (t->item[index].left != t->item[index].right) {
			t->lazy[2 * index] += t->lazy[index];
			t->lazy[2 * index + 1] += t->lazy[index];
		}
		t->lazy[index] = 0;
	}
	
	// disjoint case simply return
	if (start > t->item[index].right || end < t->item[index].left) return;
	// total overlap case return the value plus the increment to the subtrees
	if (start <= t->item[index].left && end >= t->item[index].right) {
		t->item[index].value += increment;
		if (t->item[index].left != t->item[index].right) {
			t->lazy[2 * index] += increment;
			t->lazy[2 * index + 1] += increment;
		}
		return;
	}
	propagate(t, start, end, 2 * index, increment);
	propagate(t, start, end, 2 * index + 1, increment);
	t->item[index].value = min(t->item[index * 2].value, t->item[index * 2 + 1].value);
}

// lazy propagation
void lazy_propagation(SegTree t, int start, int end, int increment) {
	if (start > end || end <= 0) return;
	propagate(t, start, end, 1, increment);
}

// update value of a key indexed i to v in the segtree 

void updatekey(SegTree t, int index, int value) {
	if (index < 0 || index > t->nkeys) return;
	fixkey(t, index, 1, value);
}

void dropsegtree(SegTree t) {
	if (t != NULL) {
		free(t->item);
		free(t->lazy);
		free(t);
	}
}


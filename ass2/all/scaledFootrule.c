// this program use "some idea" of the A* method
// searching with a dfs order but the nodes visited
// are based on the best-first order
// and in each step cutting the branches when the check condition
// fail it is g(x) + min(h(x)) >= ans
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "Pageset.h"
#include "Hashing.h"
#include "mylibrary.h"

typedef struct cost {
	float stepcost;  // cost for a single step	
	int position;    // the position of that url in the union
} Hcost;  // this struct will deal with the Hcost
// the Hcosts will be sorted based on the stepcost
int *visit;
char **bestrank;
char **tmprank;
float ans = 999999.0;
Hcost **expectation;
// int count = 0;

float min_hcost(int step, int n);
void dfs(Pageset p, int n, int step, float curr);
void debug(Pageset p, int n);
void cal_expectation(Pageset p);

int main (int argc, char *argv[]) {
	int i;
	if (argc <= 1) {
		exit(1);
	}
	Pageset p = newpageset(argc - 1);
	initpageset(p, argv, argc);
	visit = calloc(p->ranknode->total_url + 2, sizeof(int));
	bestrank = calloc(p->ranknode->total_url + 2, sizeof(char *));
	tmprank = calloc(p->ranknode->total_url + 2, sizeof(char *));
	expectation = calloc(p->ranknode->total_url + 2, sizeof(Hcost *));
	
	for (i = 0 ; i < p->ranknode->total_url; i++) {
		expectation[i] = calloc(p->ranknode->total_url + 2, sizeof(Hcost));
		bestrank[i] = NULL;
		tmprank[i] = NULL;
	}
    
	cal_expectation(p);
	
	dfs(p, p->ranknode->total_url, 0, 0.0);
	if (ans < 999999) {
		printf("%f\n", ans);
		for (i = 0 ; i < p->ranknode->total_url; i++) {
			printf("%s\n", bestrank[i]);	
		}
	}
   // printpageset(p);
    
    // debug(p, p->ranknode->total_url);
	free(visit);
	for (i = 0 ; i < p->ranknode->total_url; i++) {
		free(expectation[i]);  
		free(bestrank[i]);
		free(tmprank[i]);
	}
	free(expectation);
	free(bestrank);
	free(tmprank);
	destroypageset(p);
	// printf("%d\n", count);
	return 0;
}

int cmp(const void *pai, const void *paj) {
	Hcost *pa = (Hcost *) pai;
	Hcost *pb = (Hcost *) paj;
	return pa->stepcost > pb ->stepcost;
}
// in normal searches the f(x) = g(x) + h(x)
// while brute force search consider h(x) can be negative
// and normal search consider h(x) to be 0
// this program consider h(x) to be a number of the sum of the first colnum
// of the expectation matrix, no result can be less than g(x) + h(x)
// if g(x) + h(x) > curr_min then we could cut the search space
int dupp;   // add a global variable dup that it could be accessed by 
// functions below
float min_hcost(int step, int n) {
	int i;
	int j;
	int pos[n- step + 1];
	float cost = 0.0;
	for (i = step; i < n; i++) {
		 for (j = 0 ; j < n; j++) {
			 if (!visit[expectation[i][j].position]) {	
				pos[i - step] = expectation[i][j].position;
				cost = cost + expectation[i][j].stepcost;
				break;
			}
		}
	}
	// if has duplicate value let dupp be 1
	for (i = 0 ; i < n - step; i++) {
		for (j = i + 1; j < n - step; j++) {
			if (pos[i] == pos[j]) {
				dupp = 1;
			}
		}
	}
	return cost;
}

void cal_expectation(Pageset p) {
	int i, j, k, newid;
	for (i = 0; i < p->ranknode->total_url; i++) {
		// jth node to be in the position i+1 will have the cost expectation[i][j]  
		for (j = 0; j < p->ranknode->total_url; j++) {
			expectation[i][j].position = j+1;   // id is 1 larger than j
			expectation[i][j].stepcost = 0.0;
			for (k = 0 ; k < p->npages; k++) {
				// printf("%s\n", p->ranknode->urls[i]);
				newid =  hash_search(p->maps[k], p->ranknode->urls[j]);
			    // printf("%d %d\n", k, newid);
				if (newid > 0) {
					expectation[i][j].stepcost = expectation[i][j].stepcost + 
					fabs((newid*1.0)/(p->pages[k]->total_url*1.0) - ((i+1)*1.0) / (p->ranknode->total_url*1.0));
				    // printf("(%d, %f) \n", expectation[i][j].position, expectation[i][j].stepcost);
				}	
			}
		}
		/*for (j = 0; j < p->ranknode->total_url; j++) {
			printf("(%d, %f) ", expectation[i][j].position, expectation[i][j].stepcost);
		}
		printf("\n");*/
	}
	
	for (i = 0; i < p->ranknode->total_url; i++) {
		// sort by the hcost using descending order
		qsort(expectation[i], p->ranknode->total_url, sizeof(Hcost), cmp);
	}
	/*for (i = 0; i < p->ranknode->total_url; i++) {
	for (j = 0; j < p->ranknode->total_url; j++) {
			printf("(%d, %f) %s ", expectation[i][j].position, expectation[i][j].stepcost, p->ranknode->urls[expectation[i][j].position - 1]);
		}
		printf("\n");
	}*/
}




void dfs(Pageset p, int n, int step, float curr) {
	int i, j;
	
	float tmp = curr;
	// count++;
	// printf("%f\n", curr);
    
	dupp = 0; 
	if (curr + min_hcost(step, n) >= ans) {
		 return;
	}
	// no dup then we can directly renew the bestrank 2d string
	// and the ans
	// then cut the branch
	if (dupp == 0) {
		float cost = 0.0;
		for (i = 0 ; i < step; i++) {
				free(bestrank[i]);
				bestrank[i] = mystrdup(tmprank[i]);
		}
		cost = cost + curr;
		for (i = step; i < n; i++) {
			 for (j = 0 ; j < n; j++) {
				 if (!visit[expectation[i][j].position]) {	
					free(bestrank[i]);
					bestrank[i] = mystrdup(p->ranknode->urls[expectation[i][j].position - 1]);
					cost = cost + expectation[i][j].stepcost;
					break;
				}
			}
		}
		ans = cost;
		return;
	} 
	// save the answer
	if (step == n) {
		
		if (ans > curr) {
			ans = curr;
			for (i = 0 ; i < n; i++) {
				free(bestrank[i]);
				bestrank[i] = mystrdup(tmprank[i]);
			}
		}
		return;
	}
	
	
	for (i = 0 ; i < n; i++) {
		if (!visit[expectation[step][i].position]) {
			visit[expectation[step][i].position] = 1;
			free(tmprank[step]);
			tmprank[step] = mystrdup(p->ranknode->urls[expectation[step][i].position - 1]);
			tmp = tmp + expectation[step][i].stepcost;
			dfs(p, n, step + 1, tmp);
			tmp = curr;  // recover the previous add and then dfs again
			visit[expectation[step][i].position] = 0;
		}
	}
}

// debug function to check correctly use of the formula
void debug(Pageset p, int n) {
	float curr = 0.0;
	int i, j, newid;
	int index[5] = {1,2,5,3,4};
	// 1 2 4 5 3
	for (i = 0 ; i < n; i++) {
		// let p's step's url to have the id number i
		// increment curr and dfs
		free(tmprank[i]);
		tmprank[i] = mystrdup(p->ranknode->urls[i]);
		for (j = 0 ; j < p->npages; j++) {
			newid = hash_search(p->maps[j], p->ranknode->urls[i]);
			if (newid > -1) {
				curr = curr + fabs((newid*1.0)/(p->pages[j]->total_url*1.0) - (index[i]*1.0) / (n*1.0));
				    
			}
		}
	}
	printf("%f\n", curr);
	
}

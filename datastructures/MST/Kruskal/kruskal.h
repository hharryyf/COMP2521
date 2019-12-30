#ifndef KRU_H
#define KRU_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GraphE.h"

int find(int f[], int x);
void union_set(int f[], int a, int b);
Edge *Kruskal(Graph g);
void print(Edge mst[], int size);
#endif

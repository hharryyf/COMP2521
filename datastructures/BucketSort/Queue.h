#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int Type;

typedef struct node {
	struct node *next;
	Type value;
} Node;

typedef struct queue {
	Node *first;
	Node *last;
	int size;
} *Queue, Queuerep;

Queue newQueue();

int isEmpty(Queue q);

void enQueue(Queue q, Type element);

Type deQueue(Queue q);

void dropQueue(Queue q);
#endif

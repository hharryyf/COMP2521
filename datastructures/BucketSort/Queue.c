#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Queue.h"

Queue newQueue() {
	Queue q = calloc(1, sizeof(Queuerep));
	q->first = q->last = NULL;
	q->size = 0;
	return q;
}

static Node *newNode(Type element) {
	Node *newnd = malloc(sizeof(Node));
	newnd->next = NULL;
	newnd->value = element;
	return newnd;
}

int isEmpty(Queue q) {
	return (q->size == 0);
}

void enQueue(Queue q, Type element) {
	assert(q != NULL);
	Node *newnd = newNode(element);
	if (isEmpty(q)) {
		q->first = q->last = newnd;
		q->size = 1;
		return;
	}
	q->last->next = newnd;
	q->last = newnd;
	q->size++;
}

Type deQueue(Queue q) {
	assert(q != NULL && !isEmpty(q));
	Type nodevalue = q->first->value;
	if (q->size == 1) {
		q->size--;
		free(q->first);
		q->first = q->last = NULL;
		return nodevalue;
	}
	Node *tmp = q->first;
	q->first = q->first->next;
	free(tmp);
	q->size--;
	return nodevalue;
}

static void dropNode(Node *l) {
	if (l != NULL) {
		dropNode(l->next);
		free(l);
	}
}

void dropQueue(Queue q) {
	assert(q != NULL);
	dropNode(q->first);
	free(q);
}

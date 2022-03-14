#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct nod {
  int data;
  struct nod *prev;
  struct nod *next;
} nod_t;

typedef struct {
  nod_t *front;
  nod_t *back;
  int length;
} dequeue_t;

dequeue_t init_deq(void) {  // initializeaza coada
  dequeue_t q = {
      .front = NULL,
      .back = NULL,
      .length = 0
  };
  return q;
}

dequeue_t *alloc_deq(void) {   // aloca memorie cozii
  dequeue_t *q = (dequeue_t *)calloc(1, sizeof(dequeue_t));
  *q = init_deq();
  return q;
}

void front(dequeue_t *deq, int *value) {  // acceseaza primul element

  if( deq == NULL)
    return;
 *value = deq->front->data;
}

void push_back(dequeue_t *deq, int value) {   // adauga un element la finalul cozii
  if( deq == NULL)
    return;

  nod_t *node = (nod_t*)malloc(sizeof(nod_t));
  node->data = value;
  node->next = NULL;
  node->prev = NULL;

  if (deq->back) {
    node->prev = deq->back;
    deq->back->next = node;
  }
  deq->back = node;

  if (!deq->front)
    deq->front = deq->back;
  deq->length++;
}

void pop_front(dequeue_t *deq) {   // elimina primul element din coada

  if( deq == NULL)
    return;

  if( deq->front == NULL)
    return;
  nod_t *to_free = deq->front;

  deq->front = deq->front->next;
  if (deq->front)
    deq->front->prev = NULL;
  else
    deq->back = NULL;

  free(to_free);
  deq->length--;
}

void free_queue(dequeue_t *deq){ //elibereaza memoria alocata cozii
   while( deq->front != NULL){
      pop_front(deq);
   }
}
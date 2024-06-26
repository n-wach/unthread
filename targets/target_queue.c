// This file is part of the SV-Benchmarks collection of verification tasks:
// https://github.com/sosy-lab/sv-benchmarks
//
// SPDX-FileCopyrightText: 2011-2020 The SV-Benchmarks community
// SPDX-FileCopyrightText: The ESBMC project
//
// SPDX-License-Identifier: Apache-2.0

void reach_error() { __builtin_trap(); }

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>

#define SIZE	(20) // Other benchmarks include SIZE 200, 800
#define EMPTY	(-1)
#define FULL	(-2)
#define FALSE	(0)
#define TRUE	(1)

typedef struct {
    int element[SIZE];
    int head;
    int tail;
    int amount;
} QType;

pthread_mutex_t m;
int stored_elements[SIZE];
_Bool enqueue_flag, dequeue_flag;
QType queue;

void init(QType *q)
{
  q->head=0;
  q->tail=0;
  q->amount=0;
}

int empty(QType * q) 
{
  if (q->head == q->tail) 
  { 
    printf("queue is empty\n");
    return EMPTY;
  }
  else 
    return 0;
}

int full(QType * q) 
{
  if (q->amount == SIZE) 
  {  
	printf("queue is full\n");
	return FULL;
  } 
  else
    return 0;
}

int enqueue(QType *q, int x) 
{
  q->element[q->tail] = x;
  q->amount++;
  if (q->tail == SIZE) 
  {
    q->tail = 1;
  } 
  else 
  {
    q->tail++;
  }

	printf("enqueue\n");
  return 0;
}

int dequeue(QType *q) 
{
  int x;

  x = q->element[q->head];
  q->amount--;
  if (q->head == SIZE) 
  {
    q->head = 1;
  } 
  else 
    q->head++;

	printf("dequeue %d\n", x);
  return x;
}

void *t1(void *arg) 
{
  int value, i;

  pthread_mutex_lock(&m);
  value = abs(rand());
  if (enqueue(&queue,value)) {
    goto ERROR;
  }

  stored_elements[0]=value;
  if (empty(&queue)) {
    goto ERROR;
  }

  pthread_mutex_unlock(&m);

  for(i=0; i<(SIZE-1); i++)  
  {
    pthread_mutex_lock(&m);
    if (enqueue_flag)
    {
      value = abs(rand());
      enqueue(&queue,value);
      stored_elements[i+1]=value;
      enqueue_flag=FALSE;
      dequeue_flag=TRUE;
    }
    pthread_mutex_unlock(&m);
  }	

  return NULL;

  ERROR: {reach_error();abort();}
}

void *t2(void *arg) 
{
  int i;

  for(i=0; i<SIZE; i++)  
  {
    pthread_mutex_lock(&m);
    if (dequeue_flag)
    {
      if (!dequeue(&queue)==stored_elements[i]) {
        ERROR: {reach_error();abort();}
      }
      dequeue_flag=FALSE;
      enqueue_flag=TRUE;
    }
    pthread_mutex_unlock(&m);
  }

  return NULL;
}

int fuzz_target(void) 
{
  pthread_t id1, id2;

  enqueue_flag=TRUE;
  dequeue_flag=FALSE;

  memset(stored_elements, 0, sizeof(stored_elements));

  init(&queue);

  if (empty(&queue)!=EMPTY) {
    ERROR: {reach_error();abort();}
    goto ERROR;
  }

  pthread_mutex_init(&m, 0);

  pthread_create(&id1, NULL, t1, &queue);
  pthread_create(&id2, NULL, t2, &queue);

  pthread_join(id1, NULL);
  pthread_join(id2, NULL);

  return 0;
}

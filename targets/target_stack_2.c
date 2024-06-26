// This file is part of the SV-Benchmarks collection of verification tasks:
// https://github.com/sosy-lab/sv-benchmarks
//
// SPDX-FileCopyrightText: 2011-2020 The SV-Benchmarks community
// SPDX-FileCopyrightText: 2020 The ESBMC project
//
// SPDX-License-Identifier: Apache-2.0

extern void abort(void);
#include <assert.h>
void reach_error() { __builtin_trap(); }
void assume_abort_if_not(int cond) {
  if(!cond) {abort();}
}
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define TRUE	  (1)
#define FALSE	  (0) 
#define SIZE	  (20)
#define OVERFLOW  (-1)
#define UNDERFLOW (-2)

static int top=0;
static unsigned int arr[SIZE];
pthread_mutex_t m;
_Bool flag=FALSE;

void error(void) 
{ 
  ERROR: {reach_error();abort();}  return;
}

void inc_top(void)
{
  sched_yield();
  top++;
}

void dec_top(void)
{
  sched_yield();
  top--;
}

int get_top(void)
{
  sched_yield();
  return top;
}

int stack_empty(void)
{
  sched_yield();
  return (top==0) ? TRUE : FALSE;
}

int push(unsigned int *stack, int x)
{
  sched_yield();
  if (top==SIZE) 
  {
    printf("stack overflow\n");
    return OVERFLOW;
  } 
  else 
  {
    stack[get_top()] = x;
    inc_top();
  }
  return 0;
}

int pop(unsigned int *stack)
{
  sched_yield();
  if (top==0) 
  {
    printf("stack underflow\n");	
    return UNDERFLOW;
  } 
  else 
  {
    dec_top();
    return stack[get_top()];  
  }
  return 0;
}

void *t1(void *arg) 
{
  int i;
  unsigned int tmp;

  for(i=0; i<SIZE; i++)
  {
    pthread_mutex_lock(&m);   
    tmp = abs(rand()) % SIZE;
    assume_abort_if_not(tmp < SIZE);
    if ((push(arr,tmp)==OVERFLOW))
      error();
    pthread_mutex_unlock(&m);
  }
  return 0;
}

void *t2(void *arg) 
{
  int i;

  for(i=0; i<SIZE; i++)
  {
    pthread_mutex_lock(&m);
    if (top>0)
    {
      if ((pop(arr)==UNDERFLOW))
        error();
    }    
    pthread_mutex_unlock(&m);
  }
  return 0;
}


int fuzz_target(void) {
  top=0;
  memset(arr, 0, sizeof(arr));
  flag=FALSE;

  pthread_t id1, id2;

  pthread_mutex_init(&m, 0);

  pthread_create(&id1, NULL, t1, NULL);
  pthread_create(&id2, NULL, t2, NULL);

  pthread_join(id1, NULL);
  pthread_join(id2, NULL);

  pthread_mutex_destroy(&m);

  return 0;
}

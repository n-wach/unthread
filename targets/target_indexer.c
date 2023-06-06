// From SV-COMP 2023
// https://gitlab.com/sosy-lab/benchmarking/sv-benchmarks/-/blob/f87854c275a177e8dc3673fe6959cd89038652af/c/pthread/indexer.c

// This file is part of the SV-Benchmarks collection of verification tasks:
// https://github.com/sosy-lab/sv-benchmarks
//
// SPDX-FileCopyrightText: 2011-2020 The SV-Benchmarks community
// SPDX-FileCopyrightText: 2020 The ESBMC project
//
// SPDX-License-Identifier: Apache-2.0

void reach_error() { __builtin_trap(); }

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE  128
#define MAX   4
#define NUM_THREADS  13

int table[SIZE];
pthread_mutex_t  cas_mutex[SIZE];

pthread_t  tids[NUM_THREADS];


int cas(int * tab, int h, int val, int new_val)
{
  int ret_val = 0;
  pthread_mutex_lock(&cas_mutex[h]);
  
 
  pthread_yield();
  if ( tab[h] == val ) {
    pthread_yield();
    tab[h] = new_val;
    ret_val = 1;
  }

  pthread_mutex_unlock(&cas_mutex[h]);

  
  return ret_val;
}

void * thread_routine(void * arg)
{
  int tid;
  int m = 0, w, h;
  tid = *((int *)arg);
  
  while(1){
    pthread_yield();

    if ( m < MAX ){
      w = (++m) * 11 + tid;
    }
    else{
      pthread_exit(NULL);
    }
    
    h = (w * 7) % SIZE;
    
    if (h<0)
    {
      ERROR: {reach_error();abort();}
      ;
    }

    while ( cas(table, h, 0, w) == 0){
      pthread_yield();
      h = (h+1) % SIZE;
    }
  }

}


int fuzz_target()
{
  int i, arg;

  for (i = 0; i < SIZE; i++)
    pthread_mutex_init(&cas_mutex[i], NULL);

  for (i = 0; i < NUM_THREADS; i++){
    arg=i;
    pthread_create(&tids[i], NULL,  thread_routine, &arg);
  }

  for (i = 0; i < NUM_THREADS; i++){
    pthread_join(tids[i], NULL);
  }

  for (i = 0; i < SIZE; i++){
    pthread_mutex_destroy(&cas_mutex[i]);
  }

  // printf("OK\n");
  return 0;
}

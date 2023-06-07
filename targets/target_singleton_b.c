// This file is part of the SV-Benchmarks collection of verification tasks:
// https://github.com/sosy-lab/sv-benchmarks
//
// SPDX-FileCopyrightText: 2011-2020 The SV-Benchmarks community
// SPDX-FileCopyrightText: The CSeq project
//
// SPDX-License-Identifier: Apache-2.0

extern void abort(void);
#include <assert.h>
void reach_error() { __builtin_trap(); }

#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void __VERIFIER_assert(int expression) { if (!expression) { ERROR: {reach_error();abort();}}; return; }

char *v;

void *thread1(void * arg)
{
  v = malloc(sizeof(char));
  return 0;
}

void *thread2(void *arg)
{
  v[0] = 'X';
  return 0;
}

void *thread3(void *arg)
{
  v[0] = 'Y';
  return 0;
}

#define NUM_BEFORE 3
#define NUM_AFTER 7
#define NUM_THREADS (NUM_BEFORE + NUM_AFTER)

void *thread0(void *arg)
{
  pthread_t init, last;
  pthread_t others[NUM_THREADS];

  pthread_create(&init, 0, thread1, 0);
  pthread_join(init, 0);

  for(int i=0; i<NUM_BEFORE; i++) {
    pthread_create(&others[i], 0, thread2, 0);
  }
  pthread_create(&last, 0, thread3, 0); // this thread must run last to trigger the bug.
  for(int i=0; i<NUM_AFTER; i++) {
    pthread_create(&others[NUM_BEFORE + i], 0, thread2, 0);
  }

  for(int i=0; i<NUM_THREADS; i++) {
    pthread_join(others[i], 0);
  }

  pthread_join(last, 0);
  
  return 0;
}

int fuzz_target(void)
{
  pthread_t t;

  pthread_create(&t, 0, thread0, 0);
  pthread_join(t, 0);

  __VERIFIER_assert(v[0] == 'X'); // <-- wrong, the only thread that writes 'Y' COULD be the last to write

  free(v);
  v = NULL;

  return 0;
}

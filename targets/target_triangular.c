// This file is part of the SV-Benchmarks collection of verification tasks:
// https://github.com/sosy-lab/sv-benchmarks
//
// SPDX-FileCopyrightText: 2011-2020 The SV-Benchmarks community
// SPDX-FileCopyrightText: 2018 Dirk Beyer <https://www.sosy-lab.org>
//
// SPDX-License-Identifier: Apache-2.0

#include <pthread.h>

#define __VERIFIER_atomic_begin() pthread_yield()
#define __VERIFIER_atomic_end() pthread_yield()

extern void abort(void);
#include <assert.h>
void reach_error() { __builtin_trap(); }

#include <stdio.h>

int i = 3, j = 6;

#define NUM 20000
#define LIMIT (2*NUM+6)

void *t1(void *arg) {
  for (int k = 0; k < NUM; k++) {
    __VERIFIER_atomic_begin();
    i = j + 1;
    __VERIFIER_atomic_end();
  }
}

void *t2(void *arg) {
  for (int k = 0; k < NUM; k++) {
    __VERIFIER_atomic_begin();
    j = i + 1;
    __VERIFIER_atomic_end();
  }
}

int fuzz_target(int argc, char **argv) {
  pthread_t id1, id2;

  // printf("Starting test\n");

  pthread_create(&id1, NULL, t1, NULL);
  pthread_create(&id2, NULL, t2, NULL);

  __VERIFIER_atomic_begin();
  int condI = i >= LIMIT;
  __VERIFIER_atomic_end();

  __VERIFIER_atomic_begin();
  int condJ = j >= LIMIT;
  __VERIFIER_atomic_end();

  if (condI || condJ) {
    ERROR: {reach_error();abort();}
  }

  pthread_join(id1, NULL);
  pthread_join(id2, NULL);

  return 0;
}

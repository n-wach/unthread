#include "src/include/unthread.h"
#include <stdio.h>

void* incr(int* val) {
  while(*val < 100) {
    unthread_yield();
    *val++;
  }
}

// This is the kind of interleaving that a simple seed won't explore well...
// We hope to show that a coverage-guided fuzzer, with fine-grained control
// over the schedule, can reach the trap (a bug!)...
void* buggy(int *val) {
  unthread_yield();
  if(*val != 0) goto ok;
  unthread_yield();

  if(*val != 2) goto ok;
  unthread_yield();

  if(*val != 5) goto ok;
  unthread_yield();

  if(*val != 7) goto ok;
  unthread_yield();
    __builtin_trap();

  if(*val != 22) goto ok;
  unthread_yield();

  if(*val != 22) goto ok;
  unthread_yield();

  if(*val != 40) goto ok;
  unthread_yield();
  
  if(*val == 69) {
    // a "bug"!
    __builtin_trap();
  }

  ok:
  return 0;
}

int run_threads() {
  int val = 0;
  
  pthread_t a, b;
  unthread_create(&a, NULL, incr, &val);
  unthread_create(&b, NULL, buggy, &val);
  unthread_join(a, NULL);
  unthread_join(b, NULL);
  
  return 0;
}

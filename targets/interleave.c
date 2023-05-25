#include "src/include/pthread.h"
#include <stdio.h>

void* incr(int* val) {
  while(*val < 20) {
    pthread_yield();
    (*val)++;
  }
}

// This is the kind of interleaving that a simple seed won't explore well...
// We hope to show that a coverage-guided fuzzer, with fine-grained control
// over the schedule, can reach the trap (a bug!)...
void* buggy(int *val) {
  pthread_yield();
  if(*val != 0) return 0;
  pthread_yield();

  if(*val != 2) return 0;
  pthread_yield();

  if(*val != 5) return 0;
  pthread_yield();

  if(*val != 7) return 0;
  pthread_yield();

  if(*val != 12) return 0;
  pthread_yield();

  if(*val != 15) return 0;
  pthread_yield();

  if(*val != 15) return 0;
  pthread_yield();
  
  if(*val == 17) {
    // a "bug"!
    __builtin_trap();
  }

  return 0;
}

int interleave() {
  int val = 0;
  
  pthread_t a, b;
  pthread_create(&a, NULL, (void *(*)(void*)) incr, &val);
  pthread_create(&b, NULL, (void *(*)(void*)) buggy, &val);
  pthread_join(a, NULL);
  pthread_join(b, NULL);
  
  return 0;
}

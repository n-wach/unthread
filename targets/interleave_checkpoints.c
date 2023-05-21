#include "src/include/unthread.h"
#include <stdio.h>

void* incr(int* val) {
  while(*val < 20) {
    unthread_yield();
    (*val)++;
  }
}

// Does adding "checkpoint" functions help the fuzzer explore the space?
// (No, it doesn't seem to help much. The edges after each if statement 
// are sufficient features to explore the space.)
void ckpt0() {};
void ckpt1() {};
void ckpt2() {};
void ckpt3() {};
void ckpt4() {};
void ckpt5() {};
void ckpt6() {};
void ckpt7() {};
void ckpt8() {};
void ckpt9() {};
void ckpt10() {};

// This is the kind of interleaving that a simple seed won't explore well...
// We hope to show that a coverage-guided fuzzer, with fine-grained control
// over the schedule, can reach the trap (a bug!)...
void* buggy(int *val) {
  ckpt1();
  if(*val != 0) return 0;
  unthread_yield();

  ckpt2();
  if(*val != 2) return 0;
  unthread_yield();

  ckpt3();
  if(*val != 5) return 0;
  unthread_yield();

  ckpt4();
  if(*val != 7) return 0;
  unthread_yield();

  ckpt5();
  if(*val != 12) return 0;
  unthread_yield();

  ckpt6();
  if(*val != 15) return 0;
  unthread_yield();

  ckpt7();
  if(*val != 15) return 0;
  unthread_yield();
  
  ckpt8();
  if(*val == 17) {
    // a "bug"!
    __builtin_trap();
  }

  ckpt0();
  return 0;
}

int interleave() {
  int val = 0;
  
  pthread_t a, b;
  unthread_create(&a, NULL, incr, &val);
  unthread_create(&b, NULL, buggy, &val);
  unthread_join(a, NULL);
  unthread_join(b, NULL);
  
  return 0;
}

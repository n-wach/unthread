#include <pthread.h>
#include <stdio.h>

void* incr(int* val) {
  while(*val < 20) {
    sched_yield();
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
  sched_yield();

  ckpt2();
  if(*val != 2) return 0;
  sched_yield();

  ckpt3();
  if(*val != 5) return 0;
  sched_yield();

  ckpt4();
  if(*val != 7) return 0;
  sched_yield();

  ckpt5();
  if(*val != 12) return 0;
  sched_yield();

  ckpt6();
  if(*val != 15) return 0;
  sched_yield();

  ckpt7();
  if(*val != 15) return 0;
  sched_yield();
  
  ckpt8();
  if(*val == 17) {
    // a "bug"!
    __builtin_trap();
  }

  ckpt0();
  return 0;
}

int fuzz_target() {
  int val = 0;
  
  pthread_t a, b;
  pthread_create(&a, NULL, (void *(*)(void*)) incr, &val);
  pthread_create(&b, NULL, (void *(*)(void*)) buggy, &val);
  pthread_join(a, NULL);
  pthread_join(b, NULL);
  
  return 0;
}

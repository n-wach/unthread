#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>

#include "src/include/unthread.h"

volatile static int val = 0;

void* incr(void* arg) {
  while(val < 100) {
    pthread_yield();
    val++;
  }

  return NULL;
}

// This is the kind of interleaving that a simple seed won't explore well...
// We hope to show that a coverage-guided fuzzer, with fine-grained control
// over the schedule, can reach the trap (a bug!)...
void* buggy(void *arg) {
  pthread_yield();
  if(val != 0) goto ok;
  pthread_yield();

  if(val != 5) goto ok;
  pthread_yield();

  if(val != 5) goto ok;
  pthread_yield();

  if(val != 7) goto ok;
  pthread_yield();

  if(val != 22) goto ok;
  pthread_yield();

  if(val != 22) goto ok;
  pthread_yield();

  if(val != 40) goto ok;
  pthread_yield();
  
  if(val == 69) {
    // a "bug"!
    __builtin_trap();
    return NULL;
  }

  ok:
  return NULL;
}

int main() {
  pthread_t a, b;
  uint32_t schedule[] = {1, 2, 3};
  // unthread_configure((struct entropy_configuration){
  //   .entropy_source = ENTROPY_PRNG_SEED,
  //   .prng_seed = {
  //     .state = {0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef},
  //   },
  // });
  unthread_configure((struct entropy_configuration){
    .entropy_source = ENTROPY_SCHEDULE,
    .schedule = {
      .data = schedule,
      .data_len = sizeof(schedule) / sizeof(uint32_t),
      .end_behavior = SCHEDULE_END_LOOP,
    },
  });
  pthread_create(&a, NULL, incr, NULL);
  pthread_create(&b, NULL, buggy, NULL);
  pthread_join(a, NULL);
  pthread_join(b, NULL);

  return 0;
}

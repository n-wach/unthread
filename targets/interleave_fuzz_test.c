#define _GNU_SOURCE

#include "src/include/unthread.h"
#include <stdio.h>

volatile static int val = 0;

void* incr(void* arg) {
  while(val < 100) {
    unthread_yield();
    val++;
  }

  unthread_exit(0);
}

// This is the kind of interleaving that a simple seed won't explore well...
// We hope to show that a coverage-guided fuzzer, with fine-grained control
// over the schedule, can reach the trap (a bug!)...
void* buggy(void *arg) {
  unthread_yield();
  if(val != 0) goto ok;
  unthread_yield();

  if(val != 5) goto ok;
  unthread_yield();

  if(val != 5) goto ok;
  unthread_yield();

  if(val != 7) goto ok;
  unthread_yield();

  if(val != 22) goto ok;
  unthread_yield();

  if(val != 22) goto ok;
  unthread_yield();

  if(val != 40) goto ok;
  unthread_yield();
  
  if(val == 69) {
    // a "bug"!
    __builtin_trap();
  }

  ok:
  unthread_exit(0);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Set up thread schedule.
  // unthread_configure((struct entropy_configuration){
  //   .entropy_source = ENTROPY_PRNG_SEED,
  //   .prng_seed = {
  //     .state = {0xdeadbeef, 0xdeadbeef, 0xdeadbeef, 0xdeadbeef},
  //   },
  // });
  unthread_configure((struct entropy_configuration){
    .entropy_source = ENTROPY_SCHEDULE,
    .schedule = {
      .data = (uint32_t*) data,
      .data_len = size / sizeof(uint32_t),
      .end_behavior = SCHEDULE_END_LOOP,
    },
  });

  pthread_t a, b;
  unthread_create(&a, NULL, incr, NULL);
  unthread_create(&b, NULL, buggy, NULL);
  unthread_join(a, NULL);
  unthread_join(b, NULL);
  
  return 0;
}

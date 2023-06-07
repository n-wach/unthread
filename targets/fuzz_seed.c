#include <pthread.h>
#ifndef UNTHREAD
#error "Should be using Unthread"
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern void fuzz_target();

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Set up thread schedule.
  uint32_t state[4] = {0, 0, 0, 0};

  if(size > sizeof(state)) {
    return 0;
  }
  memcpy((void *) state, data, size);

  unthread_configure((struct entropy_configuration){
    .entropy_source = ENTROPY_PRNG_SEED,
    .prng_seed = {
      .state = {state[0], state[1], state[2], state[3]},
    },
  });

  fuzz_target();
}

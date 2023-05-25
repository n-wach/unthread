#include "src/include/pthread.h"
#include <stdio.h>

extern void interleave();

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Set up thread schedule.
  if(size != sizeof(uint32_t[4])) {
    return 0;
  }

  uint32_t *state = ((uint32_t*) data);

  unthread_configure((struct entropy_configuration){
    .entropy_source = ENTROPY_PRNG_SEED,
    .prng_seed = {
      .state = {state[0], state[1], state[2], state[3]},
    },
  });

  interleave();
}

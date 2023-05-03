#include "src/include/unthread.h"
#include <stdio.h>

extern void run_threads();

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Set up thread schedule.
  uint32_t state[4] = {0xdeadbeef, 0x1739287a, 0x88473621, 0x1010bdef};
  if(size > 4 * sizeof(uint32_t)) {
    size = 4 * sizeof(uint32_t);
  }
  memcpy(state, data, size);
  // fprintf(stderr, "state: %08x %08x %08x %08x\n", state[0], state[1], state[2], state[3]);
  unthread_configure((struct entropy_configuration){
    .entropy_source = ENTROPY_PRNG_SEED,
    .prng_seed = {
      .state = {state[0], state[1], state[2], state[3]},
    },
  });

  run_threads();
}

#include "src/include/unthread.h"
#include <stdio.h>

extern void run_threads();

int main() {
  // Set up thread schedule.

  unthread_configure((struct entropy_configuration){
    .entropy_source = ENTROPY_PRNG_SEED,
    .prng_seed = {
      .state = {0xdeadbeef, 0x1739287a, 0x88473621, 0x1010bdef},
    },
  });

  run_threads();
}

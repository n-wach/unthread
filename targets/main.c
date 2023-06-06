#include "src/include/pthread.h"
#include <stdio.h>

extern void fuzz_target();

int main() {
  // A hardcoded seed for manually testing a target.
  unthread_configure((struct entropy_configuration){
    .entropy_source = ENTROPY_PRNG_SEED,
    .prng_seed = {
      .state = {0xdeadbeef, 0x1739287a, 0x88473621, 0x1010bdef},
    },
  });

  fuzz_target();
}

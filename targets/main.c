#include <pthread.h>
#ifndef UNTHREAD
#error "Should be using Unthread"
#endif

#include <stdio.h>
#include <stdlib.h>

extern void fuzz_target();

int main() {
  srand(time(NULL));
  // For manually testing a target.
  unthread_configure((struct entropy_configuration){
    .entropy_source = ENTROPY_PRNG_SEED,
    .prng_seed = {
      .state = {rand(), rand(), rand(), rand()},
    },
  });

  fuzz_target();
}

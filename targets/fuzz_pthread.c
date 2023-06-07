#include <pthread.h>
#ifdef UNTHREAD
#error "Should NOT be using Unthread"
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern void fuzz_target();

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // No control over schedule.
  fuzz_target();
}

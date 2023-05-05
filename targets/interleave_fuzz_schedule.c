#include "src/include/unthread.h"
#include <stdio.h>

extern void run_threads();

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Set up thread schedule.
  unthread_configure((struct entropy_configuration){
    .entropy_source = ENTROPY_SCHEDULE,
    .schedule = {
      .data = (uint32_t*) data,
      .data_len = size / sizeof(uint32_t),
      .end_behavior = SCHEDULE_END_ZEROS,
    },
  });

  run_threads();
}
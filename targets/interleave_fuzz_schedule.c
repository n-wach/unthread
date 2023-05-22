#include "src/include/unthread.h"
#include <stdio.h>

extern void interleave();

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  printf("Running interleave()...\n");

  // Set up thread schedule.
  unthread_configure((struct entropy_configuration){
    .entropy_source = ENTROPY_SCHEDULE,
    .schedule = {
      .data = (uint32_t*) data,
      .data_len = size / sizeof(uint32_t),
      .end_behavior = SCHEDULE_END_ZEROS,
    },
  });


  interleave();

  return 0;
}

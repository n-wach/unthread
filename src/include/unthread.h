#ifndef UNTHREAD_UTIL
#define UNTHREAD_UTIL

#include <stdint.h>
#include <stddef.h>

struct entropy_configuration {
  enum entropy_source {
    ENTROPY_PRNG_SEED,
    ENTROPY_SCHEDULE,
    ENTROPY_ZEROS, // endless zeros (default)
  } entropy_source;
  union {
    struct {
      uint32_t state[4];
    } prng_seed;
    struct {
      uint32_t* data;
      size_t data_len;
      enum end_behavior {
        SCHEDULE_END_LOOP, // loop schedule data
        SCHEDULE_END_ZEROS, // continue with zeros
        SCHEDULE_END_EXIT, // exit with code 0
        SCHEDULE_END_TRAP, // __builtin_trap()
        // SCHEDULE_END_LONGJMP, // longjmp
      } end_behavior;
    } schedule;
  };
};

extern void unthread_configure(struct entropy_configuration config);

#endif
This project is a fork of [`mpdf/unthread`](https://github.com/mpdn/unthread), to test performance
and integrate with a modern coverage-guided fuzzer. See [`report/report.pdf`](/report/report.pdf) 
for results and more information.

Unthread
========

Unthread is an implmentation of POSIX threads designed for fuzzing and debugging of concurrent
programs. Unthread is:

- **deterministic**: Identical seeds will result in identical threading schedules.
- **fuzzable**: Unthread can be used for instrumented fuzzing of threaded code, although the
  coverage-based heuristics used by most fuzzers are likely inefficient for this.
- **strict**: Unthread only implementes the minimum required by the spec.
- **instant**: Timed locks do not wait for wall-time to pass before a lock is acquired/times out.

With some caveats:

- **Not parallel**: Unthread does not use any actual hardware threading. All threads run on a single
  core.
- **No preemption**: Control is only transfered at "yield points", which includes most pthread
  functions. This means that code cannot depend on other threads making progress by eg. waiting in a
  loop until some condition is met. `sched_yield` can be inserted in these cases to provide a
  yield point.
- **Non-conformance**: Not being a "real" pthreads implementation that integrates with the system
  means some parts of the POSIX spec are difficult/impossible to implement:
  - Only `pthread_cond_timedwait`, `pthread_cond_wait`, `pthread_join`, and `pthread_testcancel` are
    cancellation points.
  - Process shared objects are not supported (`PTHREAD_PROCESS_SHARED`).
  - Signal handling is not supported.
- **Incomplete**: Unthread is a work in progress and some pthread features have not been implemented
  yet:
  - Robust mutexes.
  - Semaphores.

Usage
-----

Unthread provides a library and a header file. We use the bazel build system. The library is
`//src:unthread`. The header overrides `pthread.h` to point all `pthread` functions to their 
`unthread` counterparts, which are implemented in the library. 

To use unthread you must overwrite `pthread.h` using a compiler flag like `-I src/include`; and
link the target to `unthread`. Example fuzz targets can be found under `targets/`. These use the
centipede fuzzer. Scripts to run the binaries can be found under `scripts/`.

The thread schedule (i.e. the sequence in which threads yield to each other) can be defined with 
`unthread_configure` in two ways:

```C
// Schedule data
unthread_configure((struct entropy_configuration){
  .entropy_source = ENTROPY_SCHEDULE,
  .schedule = {
    .data = /* pointer to array of uint32_t */,
    .data_len = /* length of array */,
    .end_behavior = SCHEDULE_END_ZEROS, 
    // Or SCHEDULE_END_LOOP, SCHEDULE_END_EXIT, SCHEDULE_END_TRAP
  },
});

// PRNG Seed
unthread_configure((struct entropy_configuration){
  .entropy_source = ENTROPY_PRNG_SEED,
  .prng_seed = {
    .state = /* nonzero uint32[4] */ {0xdeadbeef, 0x13371337, 0x0, 0x1},
  },
});
```

These should be called in your fuzzing driver, prior to calling target code.

You can also set the environment variable `UNTHREAD_VERBOSE=true` to print scheduling decisions.

Return codes
------------

Unthread may stop the application under a number of conditions:

| Retcode |                                                |
|---------|------------------------------------------------|
| 40      | The thread schedule entropy was exhausted      |
| 41      | All threads have become deadlocked             |
| 42      | An illegal operation was performed             |
| 43      | Allocation failure                             |
| 44      | Misc. failure (usually if a system call fails) |
| 45      | Failure while reading from IO                  |
| 46      | An unsupported operation was performed         |

In case that the retcode clashes with the retcode of the application, the default retcode offset of
40 can be changed by setting the `UNTHREAD_RET_OFFSET` environment variable.

Next Steps
----------

See the report.

// We are intentionally defining PTHREAD_H to prevent the inclusion of
// the system pthread.h header.
#ifndef PTHREAD_H
#define PTHREAD_H

#include "bits/pthreadtypes.h"
#include <limits.h>
#include <sched.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

#define PTHREAD_EMPTY_LIST_INITIALIZER                     \
  {                                                        \
    .len = 0,                                              \
    .cap = sizeof((struct pthread_list){}.threads.small) / \
           sizeof(*(struct pthread_list){}.threads.small), \
  }

static const struct pthread_list pthread_empty_list =
    PTHREAD_EMPTY_LIST_INITIALIZER;

#define PTHREAD_CANCELED ((void *)(-1))

#define PTHREAD_CANCEL_ASYNCHRONOUS 1
#define PTHREAD_CANCEL_ENABLE 2
#define PTHREAD_CANCEL_DEFERRED 3
#define PTHREAD_CANCEL_DISABLE 4
#define PTHREAD_CREATE_DETACHED 5
#define PTHREAD_CREATE_JOINABLE 6
#define PTHREAD_EXPLICIT_SCHED 7
#define PTHREAD_INHERIT_SCHED 8
#define PTHREAD_MUTEX_DEFAULT 9
#define PTHREAD_MUTEX_ERRORCHECK 10
#define PTHREAD_MUTEX_NORMAL 11
#define PTHREAD_MUTEX_RECURSIVE 12
#define PTHREAD_MUTEX_ROBUST 13
#define PTHREAD_MUTEX_STALLED 14
#define PTHREAD_PRIO_INHERIT 15
#define PTHREAD_PRIO_NONE 16
#define PTHREAD_PRIO_PROTECT 17
#define PTHREAD_PROCESS_SHARED 18
#define PTHREAD_PROCESS_PRIVATE 19
#define PTHREAD_SCOPE_PROCESS 20
#define PTHREAD_SCOPE_SYSTEM 21
#define PTHREAD_BARRIER_SERIAL_THREAD 22

void unthread_cleanup_push_inner(pthread_cleanup_t *cleanup);
void unthread_cleanup_pop_inner(int execute);

#define unthread_cleanup_push(routine, arg)                             \
  {                                                                    \
    pthread_cleanup_t _pthread_cleanup##__LINE__ = {(routine), (arg)}; \
    unthread_cleanup_push_inner(&_pthread_cleanup##__LINE__)

#define unthread_cleanup_pop(execute)  \
  unthread_cleanup_pop_inner(execute); \
  }

#define PTHREAD_MUTEX_INITIALIZER                                           \
  {                                                                         \
    .locked_by = NULL, .initialized = 1,                                    \
    .waiting = PTHREAD_EMPTY_LIST_INITIALIZER, .type = PTHREAD_MUTEX_NORMAL \
  }

#define PTHREAD_COND_INITIALIZER \
  { .waiting = PTHREAD_EMPTY_LIST_INITIALIZER, .initialized = 1 }

#define PTHREAD_RWLOCK_INITIALIZER                     \
  {                                                    \
    .initialized = 1, .writer = NULL,                  \
    .pending_readers = PTHREAD_EMPTY_LIST_INITIALIZER, \
    .pending_writers = PTHREAD_EMPTY_LIST_INITIALIZER  \
  }
#define PTHREAD_ONCE_INIT 1

#endif
// We are intentionally defining PTHREAD_H to prevent the inclusion of
// the system pthread.h header.
#ifndef PTHREAD_H
#define PTHREAD_H

#define UNTHREAD

#include <bits/pthreadtypes.h>
#ifndef UNTHREADTYPES
#error "Wrong header included"
#endif
#include <stdint.h>
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
        SCHEDULE_END_EXIT, // exit with EXIT_ENTROPY + UNTHREAD_RET_OFFSET
        SCHEDULE_END_TRAP, // __builtin_trap()
        // SCHEDULE_END_LONGJMP, // TODO: longjmp
        // SCHEDULE_END_CALLBACK, // TODO: callback
      } end_behavior;
    } schedule;
  };
};

void unthread_configure(struct entropy_configuration config);


int unthread_getschedparam(pthread_t thread, int *policy,
                          struct sched_param *param);
int unthread_setschedparam(pthread_t thread, int policy,
                          const struct sched_param *param);

int unthread_attr_destroy(pthread_attr_t *);
int unthread_attr_getdetachstate(const pthread_attr_t *, int *);
int unthread_attr_getguardsize(const pthread_attr_t *, size_t *);
int unthread_attr_getinheritsched(const pthread_attr_t *, int *);
int unthread_attr_getschedparam(const pthread_attr_t *, struct sched_param *);
int unthread_attr_getschedpolicy(const pthread_attr_t *, int *);
int unthread_attr_getscope(const pthread_attr_t *, int *);
int unthread_attr_getstackaddr(const pthread_attr_t *, void **);
int unthread_attr_getstacksize(const pthread_attr_t *, size_t *);
int unthread_attr_getstack(pthread_attr_t *attr, void **stackaddr,
                          size_t *stacksize);
int unthread_attr_init(pthread_attr_t *);
int unthread_attr_setdetachstate(pthread_attr_t *, int);
int unthread_attr_setguardsize(pthread_attr_t *, size_t);
int unthread_attr_setinheritsched(pthread_attr_t *, int);
int unthread_attr_setschedparam(pthread_attr_t *, const struct sched_param *);
int unthread_attr_setschedpolicy(pthread_attr_t *, int);
int unthread_attr_setscope(pthread_attr_t *, int);
int unthread_attr_setstackaddr(pthread_attr_t *, void *);
int unthread_attr_setstacksize(pthread_attr_t *, size_t);
int unthread_attr_setstack(pthread_attr_t *attr, void *stackaddr,
                          size_t stacksize);

int unthread_getattr_np(pthread_t thread, pthread_attr_t *attr);

int unthread_setcancelstate(int state, int *oldstate);
int unthread_setcanceltype(int type, int *oldtype);
int unthread_cancel(pthread_t);
int unthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *),
                   void *);
int unthread_join(pthread_t thread, void **retval);
void unthread_exit(void *retval);
int unthread_yield();
pthread_t unthread_self();
int unthread_detach(pthread_t thread);
int unthread_equal(pthread_t, pthread_t);
void unthread_testcancel(void);

int unthread_mutex_destroy(pthread_mutex_t *mutex);
int unthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int unthread_mutex_lock(pthread_mutex_t *mutex);
int unthread_mutex_trylock(pthread_mutex_t *mutex);
int unthread_mutex_unlock(pthread_mutex_t *mutex);
int unthread_mutex_timedlock(pthread_mutex_t *mutex,
                            const struct timespec *abs_timeout);
int unthread_mutex_getprioceiling(const pthread_mutex_t *mutex,
                                 int *prioceiling);
int unthread_mutex_setprioceiling(pthread_mutex_t *mutex, int prioceiling,
                                 int *old_ceiling);
int unthread_mutexattr_getprioceiling(const pthread_mutexattr_t *, int *);
int unthread_mutexattr_getprotocol(const pthread_mutexattr_t *, int *);
int unthread_mutexattr_getpshared(const pthread_mutexattr_t *, int *);
int unthread_mutexattr_gettype(const pthread_mutexattr_t *, int *);
int unthread_mutexattr_getrobust(const pthread_mutexattr_t *, int *);
int unthread_mutexattr_init(pthread_mutexattr_t *);
int unthread_mutexattr_setprioceiling(pthread_mutexattr_t *, int);
int unthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
int unthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
int unthread_mutexattr_setrobust(pthread_mutexattr_t *, int);
int unthread_mutexattr_settype(pthread_mutexattr_t *, int);
int unthread_mutexattr_destroy(pthread_mutexattr_t *);

int unthread_cond_broadcast(pthread_cond_t *);
int unthread_cond_destroy(pthread_cond_t *);
int unthread_cond_init(pthread_cond_t *, const pthread_condattr_t *);
int unthread_cond_signal(pthread_cond_t *);
int unthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *,
                           const struct timespec *);
int unthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);

int unthread_condattr_destroy(pthread_condattr_t *);
int unthread_condattr_getpshared(const pthread_condattr_t *, int *);
int unthread_condattr_getclock(pthread_condattr_t *restrict attr,
                              clockid_t *restrict clock_id);
int unthread_condattr_init(pthread_condattr_t *);
int unthread_condattr_setpshared(pthread_condattr_t *, int);
int unthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id);

int unthread_key_create(pthread_key_t *key, void (*destructor)(void *));
int unthread_key_delete(pthread_key_t key);
int unthread_setspecific(pthread_key_t key, const void *value);
void *unthread_getspecific(pthread_key_t key);

int unthread_rwlock_destroy(pthread_rwlock_t *);
int unthread_rwlock_init(pthread_rwlock_t *, const pthread_rwlockattr_t *);
int unthread_rwlock_rdlock(pthread_rwlock_t *);
int unthread_rwlock_timedrdlock(pthread_rwlock_t *, const struct timespec *);
int unthread_rwlock_timedwrlock(pthread_rwlock_t *, const struct timespec *);
int unthread_rwlock_tryrdlock(pthread_rwlock_t *);
int unthread_rwlock_trywrlock(pthread_rwlock_t *);
int unthread_rwlock_unlock(pthread_rwlock_t *);
int unthread_rwlock_wrlock(pthread_rwlock_t *);
int unthread_rwlockattr_destroy(pthread_rwlockattr_t *);
int unthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict,
                                  int *restrict);
int unthread_rwlockattr_init(pthread_rwlockattr_t *);
int unthread_rwlockattr_setpshared(pthread_rwlockattr_t *, int);

int unthread_barrierattr_init(pthread_barrierattr_t *attr);
int unthread_barrierattr_destroy(pthread_barrierattr_t *attr);
int unthread_barrierattr_getpshared(const pthread_barrierattr_t *attr,
                                   int *pshared);
int unthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared);
int unthread_barrier_destroy(pthread_barrier_t *barrier);
int unthread_barrier_init(pthread_barrier_t *restrict barrier,
                         const pthread_barrierattr_t *attr, unsigned count);
int unthread_barrier_wait(pthread_barrier_t *barrier);

int unthread_spin_init(pthread_spinlock_t *lock, int pshared);
int unthread_spin_destroy(pthread_spinlock_t *lock);
int unthread_spin_lock(pthread_spinlock_t *lock);
int unthread_spin_trylock(pthread_spinlock_t *lock);
int unthread_spin_unlock(pthread_spinlock_t *lock);

int unthread_getcpuclockid(pthread_t thread, clockid_t *clockid);
int unthread_setschedprio(pthread_t thread, int prio);
int unthread_kill(pthread_t thread, int sig);

int unthread_once(pthread_once_t *once_control, void (*init_routine)(void));

int unthread_getconcurrency(void);
int unthread_setconcurrency(int new_level);

#define pthread_getschedparam unthread_getschedparam
#define pthread_setschedparam unthread_setschedparam
#define pthread_attr_destroy unthread_attr_destroy
#define pthread_attr_getdetachstate unthread_attr_getdetachstate
#define pthread_attr_getguardsize unthread_attr_getguardsize
#define pthread_attr_getinheritsched unthread_attr_getinheritsched
#define pthread_attr_getschedparam unthread_attr_getschedparam
#define pthread_attr_getschedpolicy unthread_attr_getschedpolicy
#define pthread_attr_getscope unthread_attr_getscope
#define pthread_attr_getstackaddr unthread_attr_getstackaddr
#define pthread_attr_getstacksize unthread_attr_getstacksize
#define pthread_attr_getstack unthread_attr_getstack
#define pthread_attr_init unthread_attr_init
#define pthread_attr_setdetachstate unthread_attr_setdetachstate
#define pthread_attr_setguardsize unthread_attr_setguardsize
#define pthread_attr_setinheritsched unthread_attr_setinheritsched
#define pthread_attr_setschedparam unthread_attr_setschedparam
#define pthread_attr_setschedpolicy unthread_attr_setschedpolicy
#define pthread_attr_setscope unthread_attr_setscope
#define pthread_attr_setstackaddr unthread_attr_setstackaddr
#define pthread_attr_setstacksize unthread_attr_setstacksize
#define pthread_attr_setstack unthread_attr_setstack
#define pthread_getattr_np unthread_getattr_np
#define pthread_setcancelstate unthread_setcancelstate
#define pthread_setcanceltype unthread_setcanceltype
#define pthread_cancel unthread_cancel
#define pthread_create unthread_create
#define pthread_join unthread_join
#define pthread_exit unthread_exit
#define pthread_yield unthread_yield
#define pthread_self unthread_self
#define pthread_detach unthread_detach
#define pthread_equal unthread_equal
#define pthread_testcancel unthread_testcancel
#define pthread_mutex_destroy unthread_mutex_destroy
#define pthread_mutex_init unthread_mutex_init
#define pthread_mutex_lock unthread_mutex_lock
#define pthread_mutex_trylock unthread_mutex_trylock
#define pthread_mutex_unlock unthread_mutex_unlock
#define pthread_mutex_timedlock unthread_mutex_timedlock
#define pthread_mutex_getprioceiling unthread_mutex_getprioceiling
#define pthread_mutex_setprioceiling unthread_mutex_setprioceiling
#define pthread_mutexattr_getprioceiling unthread_mutexattr_getprioceiling
#define pthread_mutexattr_getprotocol unthread_mutexattr_getprotocol
#define pthread_mutexattr_getpshared unthread_mutexattr_getpshared
#define pthread_mutexattr_gettype unthread_mutexattr_gettype
#define pthread_mutexattr_getrobust unthread_mutexattr_getrobust
#define pthread_mutexattr_init unthread_mutexattr_init
#define pthread_mutexattr_setprioceiling unthread_mutexattr_setprioceiling
#define pthread_mutexattr_setprotocol unthread_mutexattr_setprotocol
#define pthread_mutexattr_setpshared unthread_mutexattr_setpshared
#define pthread_mutexattr_setrobust unthread_mutexattr_setrobust
#define pthread_mutexattr_settype unthread_mutexattr_settype
#define pthread_mutexattr_destroy unthread_mutexattr_destroy
#define pthread_cond_broadcast unthread_cond_broadcast
#define pthread_cond_destroy unthread_cond_destroy
#define pthread_cond_init unthread_cond_init
#define pthread_cond_signal unthread_cond_signal
#define pthread_cond_timedwait unthread_cond_timedwait
#define pthread_cond_wait unthread_cond_wait
#define pthread_condattr_destroy unthread_condattr_destroy
#define pthread_condattr_getpshared unthread_condattr_getpshared
#define pthread_condattr_getclock unthread_condattr_getclock
#define pthread_condattr_init unthread_condattr_init
#define pthread_condattr_setpshared unthread_condattr_setpshared
#define pthread_condattr_setclock unthread_condattr_setclock
#define pthread_key_create unthread_key_create
#define pthread_key_delete unthread_key_delete
#define pthread_setspecific unthread_setspecific
#define pthread_getspecific unthread_getspecific
#define pthread_rwlock_destroy unthread_rwlock_destroy
#define pthread_rwlock_init unthread_rwlock_init
#define pthread_rwlock_rdlock unthread_rwlock_rdlock
#define pthread_rwlock_timedrdlock unthread_rwlock_timedrdlock
#define pthread_rwlock_timedwrlock unthread_rwlock_timedwrlock
#define pthread_rwlock_tryrdlock unthread_rwlock_tryrdlock
#define pthread_rwlock_trywrlock unthread_rwlock_trywrlock
#define pthread_rwlock_unlock unthread_rwlock_unlock
#define pthread_rwlock_wrlock unthread_rwlock_wrlock
#define pthread_rwlockattr_destroy unthread_rwlockattr_destroy
#define pthread_rwlockattr_getpshared unthread_rwlockattr_getpshared
#define pthread_rwlockattr_init unthread_rwlockattr_init
#define pthread_rwlockattr_setpshared unthread_rwlockattr_setpshared
#define pthread_barrierattr_init unthread_barrierattr_init
#define pthread_barrierattr_destroy unthread_barrierattr_destroy
#define pthread_barrierattr_getpshared unthread_barrierattr_getpshared
#define pthread_barrierattr_setpshared unthread_barrierattr_setpshared
#define pthread_barrier_destroy unthread_barrier_destroy
#define pthread_barrier_init unthread_barrier_init
#define pthread_barrier_wait unthread_barrier_wait
#define pthread_spin_init unthread_spin_init
#define pthread_spin_destroy unthread_spin_destroy
#define pthread_spin_lock unthread_spin_lock
#define pthread_spin_trylock unthread_spin_trylock
#define pthread_spin_unlock unthread_spin_unlock
#define pthread_getcpuclockid unthread_getcpuclockid
#define pthread_setschedprio unthread_setschedprio
#define pthread_kill unthread_kill
#define pthread_once unthread_once
#define pthread_getconcurrency unthread_getconcurrency
#define pthread_setconcurrency unthread_setconcurrency


#endif
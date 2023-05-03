#ifndef UNTHREAD_UTIL
#define UNTHREAD_UTIL

#include "bits/pthreadtypes.h"
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


#endif

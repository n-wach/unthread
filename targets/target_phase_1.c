// https://github.com/mc-imperial/sctbench/blob/master/benchmarks/concurrent-software-benchmarks/phase01_bad.c


#include <pthread.h>

pthread_mutex_t  x, y;

void *thread1(void *arg)
{
  pthread_mutex_lock(&x); 
  pthread_mutex_unlock(&x);
  pthread_mutex_lock(&x);
  /* BAD: deadlock */
  // pthread_mutex_unlock(&x); 

  pthread_mutex_lock(&y);
  pthread_mutex_unlock(&y);
  pthread_mutex_lock(&y);
  pthread_mutex_unlock(&y);
}

int fuzz_target() {
  pthread_mutex_init(&x, NULL);
  pthread_mutex_init(&y, NULL);

  pthread_t t1, t2;

  pthread_create(&t1, 0, thread1, 0);
  pthread_create(&t2, 0, thread1, 0);

  pthread_join(t1, 0);
  pthread_join(t2, 0);

  return 0;
}

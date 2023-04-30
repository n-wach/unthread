#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>

// Try with: UNTHREAD_GEN=true make test-src/interleave.seeds

/*
BEGIN_TEST_SPEC
["GOT IT!", "MISSED"]
END_TEST_SPEC
*/

volatile static int val = 0;

void* incr(void* arg) {
  while(val < 100) {
    pthread_yield();
    val++;
  }

  return NULL;
}

// This is the kind of interleaving that a simple seed won't explore well...
// We hope to show that a coverage-guided fuzzer, with fine-grained control
// over the schedule, can reach CRASHED...
void* buggy(void *arg) {
  pthread_yield();
  if(val != 0) goto pass;
  pthread_yield();
  if(val != 5) goto pass;
  pthread_yield();
  if(val != 5) goto pass;
  pthread_yield();
  if(val != 7) goto pass;
  pthread_yield();
  if(val != 22) goto pass;
  pthread_yield();
  if(val != 22) goto pass;
  pthread_yield();
  if(val != 40) goto pass;
  pthread_yield();
  if(val == 69) {
    printf("GOT IT!");
    return NULL;
  }

  pass:
  printf("MISSED");
  return NULL;
}

int main() {
  pthread_t a, b;
  pthread_create(&a, NULL, incr, NULL);
  pthread_create(&b, NULL, buggy, NULL);
  pthread_join(a, NULL);
  pthread_join(b, NULL);

  return 0;
}

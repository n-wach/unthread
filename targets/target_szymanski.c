#include <pthread.h>

#include <assert.h>
void reach_error() { __builtin_trap(); }
#define __VERIFIER_atomic_begin() sched_yield()
#define __VERIFIER_atomic_end() sched_yield()

/* Testcase from Threader's distribution. For details see:
   http://www.model.in.tum.de/~popeea/research/threader
*/

#include <stdio.h>

#define printf(...)

#undef assert
#define assert(e) if (!(e)) ERROR: reach_error()

int flag1 = 0, flag2 = 0; // integer flags 
int x; // boolean variable to test mutual exclusion

void *thr1(void *_) {
    __VERIFIER_atomic_begin();
    printf("Hello from %d\n", __LINE__);
    flag1 = 1;
    __VERIFIER_atomic_end();
    __VERIFIER_atomic_begin();
    int f21 = flag2;
    __VERIFIER_atomic_end();
    while (f21 >= 3) {
        __VERIFIER_atomic_begin();
    printf("Hello from %d\n", __LINE__);
        f21 = flag2;
        __VERIFIER_atomic_end();
    };
    __VERIFIER_atomic_begin();
    flag1 = 3;
    __VERIFIER_atomic_end();
    __VERIFIER_atomic_begin();
    f21 = flag2;
    __VERIFIER_atomic_end();
    printf("Hello from %d\n", __LINE__);
    if (f21 == 1) {
        __VERIFIER_atomic_begin();
        flag1 = 2;
        __VERIFIER_atomic_end();
        __VERIFIER_atomic_begin();
        f21 = flag2;
        __VERIFIER_atomic_end();
    printf("Hello from %d\n", __LINE__);
        while (f21 != 4) {
            __VERIFIER_atomic_begin();
            f21 = flag2;
    printf("Hello from %d\n", __LINE__);
            __VERIFIER_atomic_end();
        };
    }
    __VERIFIER_atomic_begin();
    flag1 = 4;
    __VERIFIER_atomic_end();
    __VERIFIER_atomic_begin();
    printf("Hello from %d\n", __LINE__);
    f21 = flag2;
    __VERIFIER_atomic_end();
    while (f21 >= 2) {
        __VERIFIER_atomic_begin();
        f21 = flag2;
    printf("Hello from %d\n", __LINE__);
        __VERIFIER_atomic_end();
    };
    // begin critical section
    x = 0;
    assert(x<=0);
    // end critical section
    __VERIFIER_atomic_begin();
    f21 = flag2;
    __VERIFIER_atomic_end();
    printf("Hello from %d\n", __LINE__);
    while (2 <= f21 && f21 <= 3) {
        __VERIFIER_atomic_begin();
    printf("Hello from %d\n", __LINE__);
        f21 = flag2;
        __VERIFIER_atomic_end();
    };
    __VERIFIER_atomic_begin();
    flag1 = 0;
    printf("Hello from %d\n", __LINE__);
    __VERIFIER_atomic_end();
    return 0;
}

void *thr2(void *_) {
    __VERIFIER_atomic_begin();
    flag2 = 1;
    __VERIFIER_atomic_end();
    __VERIFIER_atomic_begin();
    int f12 = flag1;
    __VERIFIER_atomic_end();
    while (f12 >= 3) {
        __VERIFIER_atomic_begin();
        f12 = flag1;
        __VERIFIER_atomic_end();
    };
    __VERIFIER_atomic_begin();
    flag2 = 3;
    printf("Hello from %d\n", __LINE__);
    __VERIFIER_atomic_end();
    __VERIFIER_atomic_begin();
    f12 = flag1;
    __VERIFIER_atomic_end();
    if (f12 == 1) {
        __VERIFIER_atomic_begin();
        flag2 = 2;
        __VERIFIER_atomic_end();
        __VERIFIER_atomic_begin();
        f12 = flag1;
        __VERIFIER_atomic_end();
    printf("Hello from %d\n", __LINE__);
        while (f12 != 4) {
            __VERIFIER_atomic_begin();
            f12 = flag1;
    printf("Hello from %d\n", __LINE__);
            __VERIFIER_atomic_end();
        };
    }
    __VERIFIER_atomic_begin();
    flag2 = 4;
    __VERIFIER_atomic_end();
    __VERIFIER_atomic_begin();
    printf("Hello from %d\n", __LINE__);
    f12 = flag1;
    __VERIFIER_atomic_end();
    while (f12 >= 2) {
    printf("Hello from %d\n", __LINE__);
        __VERIFIER_atomic_begin();
        f12 = flag1;
        __VERIFIER_atomic_end();
    };
    // begin critical section
    x = 1;
    assert(x>=1);
    // end critical section
    printf("Hello from %d\n", __LINE__);
    __VERIFIER_atomic_begin();
    f12 = flag1;
    __VERIFIER_atomic_end();
    while (2 <= f12 && f12 <= 3) {
        __VERIFIER_atomic_begin();
        f12 = flag1;
        __VERIFIER_atomic_end();
    printf("Hello from %d\n", __LINE__);
    };
    __VERIFIER_atomic_begin();
    flag2 = 0;
    __VERIFIER_atomic_end();
    printf("Hello from %d\n", __LINE__);
    return 0;
}

int fuzz_target() {
  flag1 = 0, 
  flag2 = 0;
  x = 0;

  pthread_t t1, t2;
  pthread_create(&t1, 0, thr1, 0);
  pthread_create(&t2, 0, thr2, 0);
  pthread_join(t1, 0);
  pthread_join(t2, 0);
  return 0;
}

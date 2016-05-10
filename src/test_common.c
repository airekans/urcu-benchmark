#include "test_common.h"

#ifdef TEST_USE_QSBR
#include <urcu-qsbr.h>
#else
#define _LGPL_SOURCE
#if defined(TEST_USE_SIGNAL)
#define RCU_SIGNAL
#elif defined(TEST_USE_MB)
#define RCU_MB
#endif
#include <urcu.h>
#endif

#include <pthread.h>
#include <stdlib.h>

struct Foo* gs_foo = NULL;

int gs_sum = 0;
static pthread_mutex_t gs_sum_guard = PTHREAD_MUTEX_INITIALIZER;
volatile int gs_is_end;

void ReadThreadFunc()
{
    struct Foo* foo = NULL;
    int sum = 0;
    unsigned int i;
    int j;
    rcu_register_thread();
    for (i = 0; i < LOOP_TIMES; ++i) {
        for (j = 0; j < 1000; ++j) {
            rcu_read_lock();
            foo = rcu_dereference(gs_foo);
            if (foo) {
                sum += foo->a + foo->b + foo->c + foo->d;
            }
            rcu_read_unlock();
        }
        rcu_quiescent_state();
    }
    rcu_unregister_thread();
    pthread_mutex_lock(&gs_sum_guard);
    gs_sum += sum;
    pthread_mutex_unlock(&gs_sum_guard);
}

void WriteThreadFunc()
{
    int i;
    while (!gs_is_end) {
        for (i = 0; i < 1000; ++i) {
            struct Foo* foo = (struct Foo*) malloc(sizeof(struct Foo));
            foo->a = 2;
            foo->b = 3;
            foo->c = 4;
            foo->d = 5;
            rcu_xchg_pointer(&gs_foo, foo);
            synchronize_rcu();
            if (foo) {
                free(foo);
            }
        }
    }
}


#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include "common/tpool/rcu/test_common.h"

#include "common/tpool/Thread.h"
#include "common/tpool/Atomic.h"
#include "common/tpool/Mutex.h"

static Foo* GetFoo()
{
    Foo* foo = reinterpret_cast<Foo*>(malloc(sizeof(Foo)));
    foo->a = 1;
    foo->b = 2;
    foo->c = 3;
    foo->d = 4;
    return foo;
}

static pthread_mutex_t gs_sum_guard = PTHREAD_MUTEX_INITIALIZER;

void ReadOnlyThreadFunc()
{
    struct Foo* foo = NULL;
    int sum = 0;
    unsigned int i;
    int j;
    for (i = 0; i < LOOP_TIMES; ++i) {
        for (j = 0; j < 1000; ++j) {
            foo = gs_foo;
            if (foo) {
                sum += foo->a + foo->b + foo->c + foo->d;
            }
        }
    }
    pthread_mutex_lock(&gs_sum_guard);
    gs_sum += sum;
    pthread_mutex_unlock(&gs_sum_guard);
}

int main(int argc, char** argv)
{
    int thread_num = 4;
    if (argc > 1) {
        int n = atoi(argv[1]);
        if (n > 0) {
            thread_num = n;
        }
    }

    gs_foo = GetFoo();

    typedef ::boost::shared_ptr< ::tpool::Thread> ThreadPtr;
    ::std::vector<ThreadPtr> threads;

    for (int i = 0; i < thread_num; ++i) {
        threads.push_back(ThreadPtr(new ::tpool::Thread(&ReadOnlyThreadFunc)));
    }

    threads.clear();

    return 0;
}

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
        threads.push_back(ThreadPtr(new ::tpool::Thread(&ReadThreadFunc)));
    }

    {
        ::tpool::Thread write_t(&WriteThreadFunc);
        threads.clear();
        gs_is_end = true;
    }

    return 0;
}

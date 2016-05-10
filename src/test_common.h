#ifndef COMMON_TPOOL_RCU_TEST_COMMON_H_
#define COMMON_TPOOL_RCU_TEST_COMMON_H_

struct Foo 
{
    int a;
    int b;
    int c;
    int d;
};


extern struct Foo* gs_foo;

extern int gs_sum;
extern volatile int gs_is_end;
const unsigned int LOOP_TIMES = 100000;

#ifdef __cplusplus
extern "C" {
#endif

void ReadThreadFunc();
void WriteThreadFunc();

#ifdef __cplusplus
};
#endif

#endif  // COMMON_TPOOL_RCU_TEST_COMMON_H_

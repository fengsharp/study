#include <stdio.h>
#include <string>
#include <unistd.h>

#include "CurrentThread.h"
#include "Thread.h"

void mysleep(int seconds)
{
    timespec t = { seconds, 0 };
    ::nanosleep(&t, NULL);
}

void threadFunc()
{
    printf("threadFunc tid=%d\n", CurrentThread::tid());
}

void threadFunc2(int x)
{
    printf("threadFunc2 tid=%d, x=%d\n", CurrentThread::tid(), x);
}

void threadFunc3()
{
    printf("threadFunc3 tid=%d\n", CurrentThread::tid());
    mysleep(1);
}

class Foo
{
public:
    explicit Foo(double x)
        : m_dX(x)
    {
    }

    void memberFunc()
    {
        printf("Foo::memberFunc tid=%d, x=%f\n", CurrentThread::tid(), m_dX);
    }

    void memberFunc2(std::string & text)
    {
        printf("Foo::memberFunc2 tid=%d, x=%f text=%s\n", CurrentThread::tid(), m_dX, text.data());
        text = "del";
    }

    void printValue() const
    {
        printf("x: %f\n", m_dX);
    }

private:
    double m_dX;
};

int main()
{
    printf("pid=%d, tid=%d\n", ::getpid(), CurrentThread::gettid());

    Thread t1(threadFunc);
    t1.start();
    printf("t1.tid=%d\n", t1.tid());
    t1.join();

    Thread t2(std::bind(threadFunc2, 42));
    t2.start();
    printf("t2.tid=%d\n", t2.tid());
    t2.join();

    Foo foo(87.53);
    Thread t3(std::bind(&Foo::memberFunc, &foo));
    t3.start();
    t3.join();

    std::string tmpObj = "hello test";
    Thread t4(std::bind(&Foo::memberFunc2, &foo, std::ref(tmpObj)));
    t4.start();
    t4.join();
    puts(tmpObj.data());

    {
        Thread t5(threadFunc3);
        t5.start();
        // t5 may destruct eariler than thread creation.
    }
    mysleep(2);
    {
        Thread t6(threadFunc3);
        t6.start();
        mysleep(2);
        // t6 destruct later than thread creation.
    }
    sleep(2);
    printf("number of created threads %d\n", Thread::numCreated());

    return 0;
}
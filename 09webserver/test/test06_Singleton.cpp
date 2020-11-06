#include <stdio.h>

#include "CurrentThread.h"
#include "Singleton.h"
#include "Thread.h"
#include "Types.h"
#include "NoneCopyable.h"

class Test : private NoneCopyable
{
public:
    Test()
    {
        printf("Test(): tid=%d, constructing %p\n", CurrentThread::tid(), this);
    }

    ~Test()
    {
        printf("~Test(): tid=%d, destructing %p %s\n", CurrentThread::tid(), this, m_strName.c_str());
    }

    const string & name() const
    {
        return m_strName;
    }
    void setName(const string & n)
    {
        m_strName = n;
    }

private:
    string m_strName;
};

class TestNoDestroy : private NoneCopyable
{
public:
    // Tag member for Singleton<T>
    void no_destroy();

    TestNoDestroy()
    {
        printf("TestNoDestroy(): tid=%d, constructing TestNoDestroy %p\n", CurrentThread::tid(), this);
    }

    ~TestNoDestroy()
    {
        printf("~TestNoDestroy(): tid=%d, destructing TestNoDestroy %p\n", CurrentThread::tid(), this);
    }
};

void threadFunc()
{
    printf("threadFunc(): tid=%d, %p name=%s\n",
           CurrentThread::tid(),
           &Singleton<Test>::instance(),
           Singleton<Test>::instance().name().c_str());
    Singleton<Test>::instance().setName("only one, changed");
}

int main()
{
    Singleton<Test>::instance().setName("only one");
    Thread t1(threadFunc);
    t1.start();
    t1.join();
    printf("main(): tid=%d, %p name=%s\n",
           CurrentThread::tid(),
           &Singleton<Test>::instance(),
           Singleton<Test>::instance().name().c_str());
    
    Singleton<TestNoDestroy>::instance();
    printf("main(): with valgrind, you should see %zd-byte memory leak.\n", sizeof(TestNoDestroy));

    return 0;
}

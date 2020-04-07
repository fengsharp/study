#include <stdio.h>
#include <string>

#include "CurrentThread.h"
#include "Thread.h"
#include "Singleton.h"
#include "noncopyable.h"

class Test : noncopyable
{
public:
    Test()
    {
        printf("tid=%d, constructing %p\n", CurrentThread::tid(), this);
    }

    ~Test()
    {
        printf("tid=%d, destructing %p %s\n", CurrentThread::tid(), this, m_name.c_str());
    }

    const std::string& name() const { return m_name; }
    void setName(const std::string& n) { m_name = n; }

private:
    std::string m_name;
};

class TestNoDestroy : noncopyable
{
public:
    // Tag member for Singleton<T>
    void no_destroy();

    TestNoDestroy()
    {
        printf("tid=%d, constructing TestNoDestroy %p\n", CurrentThread::tid(), this);
    }

    ~TestNoDestroy()
    {
        printf("tid=%d, destructing TestNoDestroy %p\n", CurrentThread::tid(), this);
    }
};

void threadFunc()
{
    printf("tid=%d, %p name=%s\n",
            CurrentThread::tid(),
            &Singleton<Test>::instance(),
            Singleton<Test>::instance().name().c_str());
    Singleton<Test>::instance().setName("only one, changed");
}

int main() {
    Singleton<Test>::instance().setName("only one");
    Thread t1(threadFunc);
    t1.start();
    t1.join();
    printf("tid=%d, %p name=%s\n",
            CurrentThread::tid(),
            &Singleton<Test>::instance(),
            Singleton<Test>::instance().name().c_str());
    Singleton<TestNoDestroy>::instance();
    printf("with valgrind, you should see %zd-byte memory leak.\n", sizeof(TestNoDestroy));
    return 0;
}
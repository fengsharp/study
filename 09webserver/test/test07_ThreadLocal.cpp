#include <stdio.h>

#include "CurrentThread.h"
#include "Thread.h"
#include "ThreadLocal.h"
#include "NoneCopyable.h"
#include "Types.h"

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

ThreadLocal<Test> testObj1;
ThreadLocal<Test> testObj2;

void print()
{
    printf("print(): tid=%d, obj1 %p name=%s\n",
           CurrentThread::tid(),
           &testObj1.value(),
           testObj1.value().name().c_str());
    printf("print(): tid=%d, obj2 %p name=%s\n",
           CurrentThread::tid(),
           &testObj2.value(),
           testObj2.value().name().c_str());
}

void threadFunc()
{
    print();
    testObj1.value().setName("changed 1");
    testObj2.value().setName("changed 42");
    print();
}

int main()
{
    testObj1.value().setName("main one");
    print();
    Thread t1(threadFunc);
    t1.start();
    t1.join();
    testObj2.value().setName("main two");
    print();

    return 0;
}

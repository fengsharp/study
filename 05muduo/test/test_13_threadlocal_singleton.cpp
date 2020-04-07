#include <stdio.h>
#include <string>
#include <unistd.h>

#include "ThreadLocalSingleton.h"
#include "CurrentThread.h"
#include "ThreadLocal.h"
#include "Thread.h"
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
        printf("tid=%d, destructing %p %s\n", CurrentThread::tid(), this, name_.c_str());
    }
    const std::string& name() const { return name_; }
    void setName(const std::string& n) { name_ = n; }
private:
  std::string name_;
};

void threadFunc(const char* changeTo)
{
    printf("tid=%d, %p name=%s\n",
            CurrentThread::tid(),
            &ThreadLocalSingleton<Test>::instance(),
            ThreadLocalSingleton<Test>::instance().name().c_str());
    ThreadLocalSingleton<Test>::instance().setName(changeTo);
    printf("tid=%d, %p name=%s\n",
            CurrentThread::tid(),
            &ThreadLocalSingleton<Test>::instance(),
            ThreadLocalSingleton<Test>::instance().name().c_str());
    sleep(1);
    // no need to manually delete it
    // ThreadLocalSingleton<Test>::destroy();
}

int main()
{
    ThreadLocalSingleton<Test>::instance().setName("main one");
    Thread t1(std::bind(threadFunc, "thread1"));
    Thread t2(std::bind(threadFunc, "thread2"));
    t1.start();
    t2.start();
    t1.join();
    printf("tid=%d, %p name=%s\n",
            CurrentThread::tid(),
            &ThreadLocalSingleton<Test>::instance(),
            ThreadLocalSingleton<Test>::instance().name().c_str());
    t2.join();

    return 0;
}

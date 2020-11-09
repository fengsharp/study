#include <stdio.h>
#include <unistd.h>

#include "CurrentThread.h"
#include "NoneCopyable.h"
#include "Singleton.h"
#include "Thread.h"
#include "ThreadLocal.h"
#include "Types.h"
#include "ThreadLocalSingleton.h"

class Test : private NoneCopyable
{
public:
    Test()
    {
        printf("--- Test():tid=%d, constructing %p\n", CurrentThread::tid(), this);
    }

    ~Test()
    {
        printf("--- ~Test():tid=%d, destructing %p %s\n", CurrentThread::tid(), this, m_strName.c_str());
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

void threadFunc(const char* changeTo)
{
  printf("tid=%d, %p name=%s\n",
         CurrentThread::tid(),
         &ThreadLocalSingleton<Test>::instance(),
         ThreadLocalSingleton<Test>::instance().name().c_str());
  ThreadLocalSingleton<Test>::instance().setName(changeTo);
  sleep(1);
  printf("tid=%d, %p name=%s\n",
         CurrentThread::tid(),
         &ThreadLocalSingleton<Test>::instance(),
         ThreadLocalSingleton<Test>::instance().name().c_str());

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

  pthread_exit(0);
}

#include <stdio.h>
#include <unistd.h>

#include "Singleton.h"
#include "CurrentThread.h"
#include "ThreadLocal.h"
#include "Thread.h"
#include "NoneCopyable.h"
#include "Types.h"

class Test : private NoneCopyable
{
 public:
  Test()
  {
    printf("-- Test():tid=%d, constructing %p\n", CurrentThread::tid(), this);
  }

  ~Test()
  {
    printf("-- ~Test():tid=%d, destructing %p %s\n", CurrentThread::tid(), this, m_strName.c_str());
  }

  const string& name() const { return m_strName; }
  void setName(const string& n) { m_strName = n; }

 private:
  string m_strName;
};

#define STL Singleton<ThreadLocal<Test> >::instance().value()

void print()
{
  printf("print():tid=%d, %p %p name=%s\n",
         CurrentThread::tid(),
         &Singleton<ThreadLocal<Test> >::instance(),
         &STL,
         STL.name().c_str());
}

void threadFunc(const char* changeTo)
{
  print();
  STL.setName(changeTo);
  sleep(1);
  print();
}

int main()
{
  STL.setName("main one");
  Thread t1(std::bind(threadFunc, "thread1"));
  Thread t2(std::bind(threadFunc, "thread2"));
  t1.start();
  t2.start();
  t1.join();
  print();
  t2.join();
  pthread_exit(0);
}

#include <stdio.h>

#include <string>

#include <unistd.h>

#include "Singleton.h"
#include "Thread.h"
#include "CurrentThread.h"

class Test : private NonCopyable
{
public:
    void setName(const std::string& name)
    {
        m_strName = name;
    }

    void print()
    {
        printf("pid: %d thread: %d Test %p name:%s\n", ::getpid(), CurrentThread::tid(), this, m_strName.data());
    }

private:
    std::string m_strName;
};

void threadFunc()
{
    Singleton<Test>::instance()->setName("threadFunc");
    Singleton<Test>::instance()->print();
}

int main()
{
    Singleton<Test>::instance()->setName("main");
    Singleton<Test>::instance()->print();

    Thread th(std::bind(threadFunc), "th1");
    th.start();
    th.join();

    Singleton<Test>::instance()->print();

    puts("=== ok. ===");

    return 0;
}
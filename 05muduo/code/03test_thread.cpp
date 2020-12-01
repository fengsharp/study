#include <stdio.h>

#include "CurrentThread.h"
#include "Thread.h"

class Foo
{
public:
    explicit Foo(double x)
        : m_dX(x)
    {
    }

    void memberFunc()
    {
        printf("tid=%d, Foo::m_dx=%f\n", CurrentThread::tid(), m_dX);
    }

    void memberFunc2(const std::string & text)
    {
        printf("tid=%d, Foo::m_dx=%f text=%s\n", CurrentThread::tid(), m_dX, text.data());
    }

    void setX(double x)
    {
        m_dX = x;
    }
private:
    double m_dX;
};

int main()
{
    Foo foo(100.0);
    Thread t1 = Thread(std::bind(&Foo::memberFunc, &foo), "t1");
    t1.start();
    t1.join();

    std::string str2 = "hello2";
    Thread t2 = Thread(std::bind(&Foo::memberFunc2, &foo, str2), "t2");
    foo.setX(200.0);
    str2.append("_update");
    t2.start();
    t2.join();

    std::string str3 = "hello3";
    Thread t3 = Thread(std::bind(&Foo::memberFunc2, std::ref(foo), std::ref(str3)), "t3");
    foo.setX(300.0);
    str3.append("_update");
    t3.start();
    t3.join();

    return 0;
}
#include "CurrentThread.h"
#include "Exception.h"

#include <functional>
#include <stdio.h>
#include <vector>

class Bar
{
public:
    void test(std::vector<std::string> names = {})
    {
        printf("Stack:\n%s\n", CurrentThread::stackTrace(true).c_str());
        [] {
            printf("Stack inside lambda:\n%s\n", CurrentThread::stackTrace(true).c_str());
        }();
        std::function<void()> func([] {
            printf("Stack inside std::function:\n%s\n", CurrentThread::stackTrace(true).c_str());
        });
        func();

        func = std::bind(&Bar::callback, this);
        func();

        throw Exception("oops");
    }

private:
    void callback()
    {
        printf("Stack inside std::bind:\n%s\n", CurrentThread::stackTrace(true).c_str());
    }
};

void foo()
{
    Bar b;
    b.test();
}

int main()
{
    try
    {
        foo();
    }
    catch (const Exception & ex)
    {
        printf("---reason: %s\n", ex.what());
        printf("---stack trace:\n%s\n", ex.stackTrace());
    }
}

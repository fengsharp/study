#include <stdio.h>

#include <vector>

#include "TestMutex.h"

class Foo
{
public:
    void doit() const;
};

threadtest::MutexLock g_mutex;
std::vector<Foo> g_vecFoos;

void post(const Foo & f)
{
    threadtest::MutexLockGuard lock(g_mutex);
    g_vecFoos.push_back(f);
}

void traverse()
{
    threadtest::MutexLockGuard lock(g_mutex);
    for (const Foo & item : g_vecFoos)
    {
        item.doit();
    }
}

void Foo::doit() const
{
    Foo f;
    post(f);
}

int main()
{
    Foo f;
    post(f);
    traverse();

    return 0;
}
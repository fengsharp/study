#include <stdio.h>
#include <assert.h>

#include <memory>
#include <vector>

#include "TestMutex.h"

class Foo
{
public:
    Foo()
    {
        printf("    Foo() %p\n", this);
    }

    ~Foo()
    {
        printf("    ~Foo() %p\n", this);
    }

    Foo(const Foo & other)
    {
        printf("    Foo(const Foo& other) %p, other %p\n", this, &other);
    }

public:
    void doit() const;
};

threadtest::MutexLock g_mutex;
std::shared_ptr<std::vector<Foo>> g_pVecFoos;

void post(const Foo & f)
{
    threadtest::MutexLockGuard lock(g_mutex);
    if (!g_pVecFoos.unique())
    {
        printf("post1 g_pVecFoos:%p vector-size:%lu\n", g_pVecFoos.get(), g_pVecFoos->size());
        g_pVecFoos.reset(new std::vector<Foo>(g_pVecFoos->begin(), g_pVecFoos->end()));
        g_pVecFoos->reserve(10);
        printf("post2 g_pVecFoos:%p vector-size:%lu\n", g_pVecFoos.get(), g_pVecFoos->size());
    }

    g_pVecFoos->push_back(f);
    if (g_pVecFoos->size() > 1)
    {
        // printf("# g_pVecFoos use_count: %lu vector-size:%lu\n", g_pVecFoos.use_count(), g_pVecFoos->size());
        printf("post3 g_pVecFoos:%p size:%lu, 01:%p 02:%p\n", g_pVecFoos.get(), g_pVecFoos->size(), &g_pVecFoos->at(0), &g_pVecFoos->at(1));
    }
    else
    {
        printf("post4 g_pVecFoos:%p size:%lu, 01:%p\n", g_pVecFoos.get(), g_pVecFoos->size(), &g_pVecFoos->at(0));
    }
}

void traverse()
{
    std::shared_ptr<std::vector<Foo>> copyPVecFoos;
    {
        threadtest::MutexLockGuard lock(g_mutex);
        copyPVecFoos = g_pVecFoos;
        assert(!copyPVecFoos.unique());
    }

    for (const Foo & item : *copyPVecFoos)
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
    g_pVecFoos.reset(new std::vector<Foo>());

    {
        Foo f;
        post(f);
    }

    printf("\n--------------------------\n");
    traverse();

    return 0;
}
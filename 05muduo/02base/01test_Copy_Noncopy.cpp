#include <stdio.h>

#include "Copyable.h"
#include "NonCopyable.h"

class TestCopyable : public Copyable
{
public:
    TestCopyable()
    {
        printf("TestCopyable new object %p\n", this);
    }
    ~TestCopyable()
    {
        printf("TestCopyable object destructor %p\n", this);
    }
};

class TestNonCopyable : private NonCopyable
{
public:
    TestNonCopyable()
    {
        printf("TestNonCopyable new object %p\n", this);
    }
    ~TestNonCopyable()
    {
        printf("TestNonCopyable object destructor %p\n", this);
    }
};

int main()
{
    TestCopyable cp1;
    TestCopyable cp2 = cp1;

    TestNonCopyable ncp1;
    // TestNonCopyable ncp2 = ncp1; - copy constructor of 'TestNonCopyable' is implicitly deleted

    return 0;
}
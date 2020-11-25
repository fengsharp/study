#include <stdio.h>

#include "Mutex.h"


class Request
{
public:
    void process()
    {
        MutexLockGuard lock(m_mutex);
        print();
    }

    void print()
    {
        MutexLockGuard lock(m_mutex);
    }
private:
    mutable MutexLock m_mutex;
};

int main()
{
    Request req;
    req.process();
    
    return 0;
}
#ifndef COUNT_DOWN_LATCH_H_
#define COUNT_DOWN_LATCH_H_

#include "Mutex.h"
#include "Condition.h"

class CountDownLatch : private NoneCopyable
{
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();
    int getCount() const;
private:
    mutable MutexLock m_mutex;
    Condition m_condition GUARDED_BY(m_mutex);
    int m_count GUARDED_BY(m_mutex);
};

#endif
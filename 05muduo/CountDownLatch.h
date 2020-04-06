#ifndef MUDUO_BASE_COUNTDOWNLATCH_H
#define MUDUO_BASE_COUNTDOWNLATCH_H

#include "MutexLock.h"
#include "Condition.h"
#include "noncopyable.h"

class CountDownLatch : noncopyable
{
public:
  explicit CountDownLatch(int count);

  void wait();
  void countDown();
  int getCount() const;

private:
    mutable MutexLock m_mutex;
    Condition m_conditon;
    int m_count;
};


#endif
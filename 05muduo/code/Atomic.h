#ifndef ATOMIC_H_
#define ATOMIC_H_

#include "Noncopyable.h"

#include <stdint.h>

template <typename T>
class AtomicIntegerT : private NonCopyable
{
public:
    AtomicIntegerT()
        : m_value(0)
    {
    }

    T get()
    {
        // 无论是否修改m_value的值，都返回修改前的值
        return __sync_val_compare_and_swap(&m_value, 0, 0);
    }

    T getAndAdd(T x)
    {
        return __sync_fetch_and_add(&m_value, x);
    }

    T addAndGet(T x)
    {
        return getAndAdd(x) + x;
    }

    T incrementAndGet()
    {
        return addAndGet(1);
    }

    T decrementAndGet()
    {
        return addAndGet(-1);
    }

    void add(T x)
    {
        getAndAdd(x);
    }

    void increment()
    {
        incrementAndGet();
    }

    void decrement()
    {
        decrementAndGet();
    }

    T getAndSet(T newValue)
    {
        return __sync_lock_test_and_set(&m_value, newValue);
    }

private:
    volatile T m_value;
};

typedef AtomicIntegerT<int32_t> AtomicInt32;
typedef AtomicIntegerT<int64_t> AtomicInt64;

#endif
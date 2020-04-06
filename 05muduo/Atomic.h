#ifndef MUDUO_BASE_ATOMIC_H
#define MUDUO_BASE_ATOMIC_H

#include <stdint.h>

#include "noncopyable.h"

template<typename T>
class AtomicIntegerT : noncopyable
{
public:
    AtomicIntegerT()
        : m_value(0)
    {
    }

    T get() {
        return __atomic_load_n(&m_value, __ATOMIC_SEQ_CST);
    }

    T getAndAdd(T x) {
        return __atomic_fetch_add(&m_value, x, __ATOMIC_SEQ_CST);
    }

    T addAndGet(T x) {
        return getAndAdd(x) + x;
    }

    T incrementAndGet() {
        return addAndGet(1);
    }

    T decrementAndGet() {
        return addAndGet(-1);
    }

    void add(T x) {
        getAndAdd(x);
    }

    void increment() {
        incrementAndGet();
    }

    void decrement() {
        decrementAndGet();
    }

    T getAndSet(T newValue) {
        return __atomic_exchange_n(&m_value, newValue, __ATOMIC_SEQ_CST);
    }

private:
    volatile T m_value;
};

typedef AtomicIntegerT<int32_t> AtomicInt32;
typedef AtomicIntegerT<int64_t> AtomicInt64;

#endif
#ifndef ATOMIC_H_
#define ATOMIC_H_

#include <stdint.h>

#include "NonCopyable.h"

/*
https://code.i-harness.com/zh-CN/docs/gcc~7/_005f_005fatomic-builtins

内置函数： 类型 __atomic_load_n （ 类型 * ptr，int存储器）
这个内置函数实现了一个原子加载操作。 它返回* ptr的内容。

内置函数：void __atomic_load （ type * ptr， type * ret，int memorder）
这是原子加载的通用版本。 它返回* ret中* ptr的内容。

内置函数：void __atomic_store_n （ type * ptr， type val，int memorder）
这个内置函数实现了一个原子存储操作。 它将val写入* ptr 。

内置函数：void __atomic_store （ type * ptr， type * val，int memorder）
这是原子store的通用版本。 它将* val的值存储到* ptr 。

内置函数： 类型 __atomic_exchange_n （ 类型 * ptr， 类型 val，int存储器）
该内置函数实现原子交换操作。 它将val写入* ptr ，并返回* ptr的前一个内容。

内建函数：void __atomic_exchange （ type * ptr， type * val， type * ret，int memorder）
这是原子交换的通用版本。 它将* val的内容存储到* ptr 。 * ptr的原始值被复制到* ret 

内置函数：bool __atomic_compare_exchange_n(类型 * ptr，类型 * expected，类型 desired，bool weak，int success_memorder，int failure_memorder）
该内置函数实现了原子比较和交换操作。 
比较* ptr的内容和* expected的内容。 如果相等，则该操作是将读入 - 修改 - 写入操作写入* ptr 所需的操作。 
如果它们不相等，则操作是读取并且* ptr的当前内容被写入* expected 

内置函数： 类型 __atomic_add_fetch （ 类型 * ptr， 类型 val，int存储器）
内置函数： 类型 __atomic_sub_fetch （ 类型 * ptr， 类型 val，int存储器）
内置函数： 类型 __atomic_and_fetch （ 类型 * ptr， 类型 val，int存储器）
内置函数： 类型 __atomic_xor_fetch （ 类型 * ptr， 类型 val，int存储器）
内建函数： type __atomic_or_fetch （ type * ptr， type val，int memorder）
内置函数： 类型 __atomic_nand_fetch （ 类型 * ptr， 类型 val，int存储器）

内置函数： 类型 __atomic_fetch_add （ 类型 * ptr， 类型 val，int存储器）
内置函数： type __atomic_fetch_sub （ type * ptr， type val，int memorder）
内置函数： 键入 __atomic_fetch_and （ type * ptr， type val，int memorder）
内置函数： 类型 __atomic_fetch_xor （ 类型 * ptr， 类型 val，int存储器）
内置函数： 类型 __atomic_fetch_or （ 类型 * ptr， 类型 val，int存储器）
内置函数： 类型 __atomic_fetch_nand （ 类型 * ptr， 类型 val，int存储器）

*/
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
        return __atomic_load_n(&m_value, __ATOMIC_SEQ_CST);
    }

    T getAndAdd(T x)
    {
        return __atomic_fetch_add(&m_value, x, __ATOMIC_SEQ_CST);
    }

    T addAndGet(T x)
    {
        return getAndAdd(x) + x;
    }

    T incrementAndGet()
    {
        return addAndGet(1);
    }

    T decrementAdnGet()
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
        decrementAdnGet();
    }

    T getAndSet(T newValue)
    {
        return __atomic_exchange_n(&m_value, newValue, __ATOMIC_SEQ_CST);
    }

private:
    // 不被优化掉，cpu和内存读写变量
    volatile T m_value;
};

typedef AtomicIntegerT<int32_t> AtomicInt32;
typedef AtomicIntegerT<int64_t> AtomicInt64;

#endif
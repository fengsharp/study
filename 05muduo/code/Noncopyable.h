#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_

/// 子类不可拷贝赋值
class NonCopyable
{
public:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable(const NonCopyable &&) = delete;
    NonCopyable & operator=(const NonCopyable &) = delete;
    NonCopyable & operator=(const NonCopyable &&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

#endif
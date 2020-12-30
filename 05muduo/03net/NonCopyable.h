#ifndef NON_COPYABLE_H_
#define NON_COPYABLE_H_

class NonCopyable
{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

public:
    NonCopyable(NonCopyable &) = delete;
    NonCopyable(NonCopyable &&) = delete;
    NonCopyable & operator=(NonCopyable &) = delete;
    NonCopyable & operator=(NonCopyable &&) = delete;
};

#endif
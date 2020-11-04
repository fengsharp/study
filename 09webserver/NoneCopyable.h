#ifndef NONE_COPYABLE_H_
#define NONE_COPYABLE_H_

/*
    一旦继承，子类中不会自动生成 拷贝构造和赋值
    内部如果有指针，自定义内存管理，则继承后，酌情考虑实现拷贝构造和赋值
*/
class NoneCopyable
{
protected:
    NoneCopyable() = default;
    ~NoneCopyable() = default;

public:
    NoneCopyable(const NoneCopyable &) = delete;
    NoneCopyable & operator=(const NoneCopyable &) = delete;
};

#endif
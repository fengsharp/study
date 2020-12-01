#ifndef COPYABLE_H_
#define COPYABLE_H_

/// 标明继承子类值类型，可拷贝
/// 子类中不要禁用拷贝构造，赋值构造
class Copyable
{
protected:
    Copyable() = default;
    ~Copyable() = default;
};

#endif
#ifndef COPYABLE_H_
#define COPYABLE_H_

/*
    child class should be a value type.
*/
class Copyable
{
protected:
    Copyable() = default;
    ~Copyable() = default;
};

#endif
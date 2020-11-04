#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <exception>
#include "Types.h"

class Exception : public std::exception
{
public:
    Exception(string what);
    ~Exception() noexcept override = default;

    const char* what() const noexcept override
    {
        return m_strMessage.c_str();
    }

    const char* stackTrace() const noexcept
    {
        return m_strStack.c_str();
    }
private:
    string m_strMessage;
    string m_strStack;
};

#endif
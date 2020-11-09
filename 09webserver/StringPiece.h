#ifndef STRING_PICE_H_
#define STRING_PICE_H_

#include <string_view>

#include "Types.h"

class StringArg
{
public:
    StringArg(const char * str)
        : m_szString(str)
    {
    }

    StringArg(const string & str)
        : m_szString(str.data())
    {
    }

    const char * c_str() const
    {
        return m_szString;
    }

private:
    const char * m_szString;
};

typedef std::string_view StringPiece;

#endif
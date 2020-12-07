#ifndef STRING_PIECE_H_
#define STRING_PIECE_H_

#include <string_view>

#include "Types.h"

class StringArg
{
public:
    StringArg(const char * str)
        : m_str(str)
    {
    }

    StringArg(const std::string & str)
        : m_str(str.data())
    {
    }

    const char * data() const
    {
        return m_str;
    }
private:
    const char * m_str;
};

typedef std::string_view StringPiece;

#endif
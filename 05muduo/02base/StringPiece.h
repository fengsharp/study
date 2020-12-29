#ifndef STRING_PIECE_H_
#define STRING_PIECE_H_

#include <string>
#include <string_view>

class StringArg
{
public:
    StringArg(const char * szStr)
        : m_pStrData(szStr)
    {
    }

    StringArg(const std::string & str)
        : m_pStrData(str.data())
    {
    }

    const char * data() const
    {
        return m_pStrData;
    }

private:
    const char * m_pStrData;
};

typedef std::string_view StringPiece;

#endif
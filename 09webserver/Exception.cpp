#include "Exception.h"
#include "CurrentThread.h"

Exception::Exception(string what)
    : m_strMessage(std::move(what)),
    m_strStack(CurrentThread::stackTrace(false))
{

}
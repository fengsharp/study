#include "Exception.h"

#include "CurrentThread.h"

Exception::Exception(std::string msg)
  : m_message(std::move(msg)),
    m_stack(CurrentThread::stackTrace(/*demangle=*/false))
{
}
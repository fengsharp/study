#ifndef MUDUO_BASE_CURRENTTHREAD_H
#define MUDUO_BASE_CURRENTTHREAD_H

#include <string>

namespace CurrentThread
{
    // demangle: C++编译以后的函数名
    std::string stackTrace(bool demangle);


}

#endif
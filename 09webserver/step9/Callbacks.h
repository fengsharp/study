#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <functional>
#include <memory>

#include "Timestamp.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

// should really belong to base/Types.h, but <memory> is not included there.

template<typename T>
inline T* get_pointer(const std::shared_ptr<T>& ptr)
{
  return ptr.get();
}

template<typename T>
inline T* get_pointer(const std::unique_ptr<T>& ptr)
{
  return ptr.get();
}

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void()> TimerCallback;

typedef std::function<void(const TcpConnectionPtr &)> ConnectionCallback;

typedef std::function<void(const TcpConnectionPtr &, const char * data, ssize_t len)> MessageCallback;



#endif
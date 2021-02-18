#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <functional>
#include <memory>

typedef std::function<void()> LoopFunctor;

typedef std::function<void()> TimerCallback;

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void(const TcpConnectionPtr &)> ConnectionCallback;
typedef std::function<void(const TcpConnectionPtr &, const char * data, ssize_t len)> MessageCallback;

#endif
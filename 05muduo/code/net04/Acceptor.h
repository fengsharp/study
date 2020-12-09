#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include <functional>

#include "Noncopyable.h"
#include "Channel.h"
#include "Socket.h"
#include "Timestamp.h"

class EventLoop;
class InetAddress;

// 接受和关闭
class Acceptor : private NonCopyable
{
public:
    typedef std::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

    Acceptor(EventLoop * pLoop, const InetAddress & listenAddr);

    void setNewConnectionCallback(const NewConnectionCallback & cb)
    {
        m_newConnectionCallback = cb;
    }

    bool listenning() const
    {
        return m_bListenning;
    }

    void listen();

private:
    void handleRead(Timestamp receiveTime);

private:
    EventLoop * m_pLoop;
    Socket m_acceptSocket;
    Channel m_acceptChannel;
    NewConnectionCallback m_newConnectionCallback;

    bool m_bListenning;
};

#endif
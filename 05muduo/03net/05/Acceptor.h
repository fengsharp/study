#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include <functional>

#include "Channel.h"
#include "NonCopyable.h"
#include "Socket.h"

class EventLoop;
class InetAddress;

class Acceptor : private NonCopyable
{
public:
    typedef std::function<void(int sockfd, const InetAddress&)> NewConnectionCallback;
    Acceptor(EventLoop * pLoop, const InetAddress & listenAddr);

    void setNewCoonectionCallback(const NewConnectionCallback & cb)
    {
        m_newCoonectionCallback = cb;
    }

    bool listenning() const
    {
        return m_bListenning;
    }

    void listen();

private:
    void handleRead(Timestamp receivedTime);
private:
    EventLoop * m_pLoop;
    Socket m_acceptSocket;
    Channel m_acceptChannel;
    NewConnectionCallback m_newCoonectionCallback;
    bool m_bListenning;
};

#endif
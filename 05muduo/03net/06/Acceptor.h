#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include <functional>

#include "Channel.h"
#include "NonCopyable.h"
#include "Socket.h"

class EvnetLoop;
class InetAddress;

class Acceptor : private NonCopyable
{
public:
    typedef std::function<void(int sockfd, const InetAddress &)> NewConnectionCallback;

    Acceptor(EventLoop * pLoop, const InetAddress & listenAddr);
    
    void setNewConnectionCallback(const NewConnectionCallback & cb)
    {
        m_newConntionCallback = cb;
    }

    void listen();
    bool listenning() const
    {
        return m_bListenning;
    }

private:
    void handleRead(Timestamp receivedTime);

private:
    EventLoop * m_pLoop;
    Socket m_acceptSocket;
    Channel m_acceptChannel;
    bool m_bListenning;

    NewConnectionCallback m_newConntionCallback;
};



#endif
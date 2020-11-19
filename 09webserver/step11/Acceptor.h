#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include <functional>

#include "Channel.h"
#include "Socket.h"

class EventLoop;
class InetAddress;

class Acceptor
{
public:
    typedef std::function<void(int sockfd, const InetAddress &)> NewConnectionCallback;

public:
    Acceptor(EventLoop * pLoop, const InetAddress & listenAddr, bool reuseport = false);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback & cb)
    {
        m_newConnectionCallback = cb;
    }

    bool listenning() const
    {
        return m_bListing;
    }

    void listen();

private:
    void handleRead();

private:
    EventLoop * m_pLoop;
    Socket m_acceptSocket;
    Channel m_acceptChannel;
    NewConnectionCallback m_newConnectionCallback;
    bool m_bListing;
    int m_idleFd;
};

#endif
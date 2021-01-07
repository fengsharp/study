#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include <memory>
#include <map>

#include "NonCopyable.h"
#include "Callbacks.h"
#include "Acceptor.h"
#include "TcpConnection.h"

class EventLoop;

class TcpServer : private NonCopyable
{
public:
    TcpServer(EventLoop * pLoop, const InetAddress & listenAddr);
    ~TcpServer();

    void start();

    void setConnectionCallback(const ConnectionCallback & cb)
    {
        m_connectionCallback = cb;
    }

    void setMessageCallback(const MessageCallback & cb)
    {
        m_messageCallback = cb;
    }

private:
    void newConnection(int sockfd, const InetAddress & peerAddr);

private:
    EventLoop * m_pLoop;
    const std::string m_name;
    Acceptor m_acceptor;
    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
    bool m_bStarted;
    int m_nextConnId;
    std::map<std::string, TcpConnectionPtr> m_mapConnections;
};

#endif
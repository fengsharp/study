#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include <map>

#include "Atomic.h"
#include "TcpConnection.h"
#include "Types.h"

class Acceptor;
class EventLoop;

class TcpServer : private NoneCopyable
{
public:
    TcpServer(EventLoop * pLoop, const InetAddress & listenAddr, const string & nameArg);
    ~TcpServer();

    const string & ipPort() const
    {
        return m_strIpPort;
    }

    const string & name() const
    {
        return m_strName;
    }

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
    void newConnection(int sockfd, const InetAddress& peerAddr);
    
private:
    EventLoop * m_pLoop;
    const string m_strIpPort;
    const string m_strName;
    std::unique_ptr<Acceptor> m_pAcceptor;
    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
    AtomicInt32 m_bStarted;
    int m_nextConnId;
    std::map<string, TcpConnectionPtr> m_mapConnection;
};

#endif
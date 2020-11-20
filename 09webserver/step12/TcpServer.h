#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include <map>

#include "Atomic.h"
#include "TcpConnection.h"
#include "Types.h"

class Acceptor;
class EventLoop;
class EventLoopThreadPool;

class TcpServer : private NoneCopyable
{
public:
    typedef std::function<void(EventLoop *)> ThreadInitCallback;

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

    void setThreadNum(int numThreads);
    void setThreadInitCallback(const ThreadInitCallback & cb)
    {
        m_threadInitCallback = cb;
    }
    /// valid after calling start()
    std::shared_ptr<EventLoopThreadPool> threadPool()
    {
        return m_pThreadPool;
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

    void setWriteCompleteCallback(const WriteCompleteCallback & cb)
    {
        m_writeCompleteCallback = cb;
    }

private:
    void newConnection(int sockfd, const InetAddress & peerAddr);
    void removeConnection(const TcpConnectionPtr & conn);
    void removeConnectionInLoop(const TcpConnectionPtr & conn);

private:
    EventLoop * m_pLoop;
    const string m_strIpPort;
    const string m_strName;
    std::unique_ptr<Acceptor> m_pAcceptor;
    std::shared_ptr<EventLoopThreadPool> m_pThreadPool;
    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
    WriteCompleteCallback m_writeCompleteCallback;
    ThreadInitCallback m_threadInitCallback;
    AtomicInt32 m_bStarted;
    int m_nextConnId;
    std::map<string, TcpConnectionPtr> m_mapConnection;
};

#endif
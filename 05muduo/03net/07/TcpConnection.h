#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <any>
#include <memory>
#include <string>

#include "Callbacks.h"
#include "Channel.h"
#include "InetAddress.h"
#include "NonCopyable.h"
#include "Socket.h"
#include "Timestamp.h"

class EventLoop;
class TcpConnection : private NonCopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop * pLoop, const std::string & name, int sockfd, const InetAddress & localAddr, const InetAddress & peerAddr);
    ~TcpConnection();

    EventLoop * getLoop() const
    {
        return m_pLoop;
    }

    const std::string & name() const
    {
        return m_name;
    }

    const InetAddress & localAddress() const
    {
        return m_localAddr;
    }

    const InetAddress & peerAddress() const
    {
        return m_peerAddr;
    }

    bool connected() const
    {
        return m_state == StateE::kConnected;
    }

    void setConnectionCallback(const ConnectionCallback & cb)
    {
        m_connectionCallback = cb;
    }

    void setMessageCallback(const MessageCallback & cb)
    {
        m_messageCallback = cb;
    }

    void connectEstablished();
private:
    enum StateE
    {
        kConnecting,
        kConnected,
    };

    void setState(StateE status)
    {
        m_state = status;
    }

    void handleRead(Timestamp receivedTime);
private:
    EventLoop * m_pLoop;
    Socket m_connSocket;
    Channel m_connChannel;

    std::string m_name;
    StateE m_state;
    InetAddress m_localAddr;
    InetAddress m_peerAddr;

    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

#endif
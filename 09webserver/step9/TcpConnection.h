#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <any>
#include <memory>

#include "Callbacks.h"
#include "InetAddress.h"
#include "NoneCopyable.h"
#include "StringPiece.h"
#include "Types.h"

struct tcp_info;

class Channel;
class EventLoop;
class Socket;

class TcpConnection : private NoneCopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop * pLoop, const string & name, int sockfd, const InetAddress & localAddr, const InetAddress & peerAddr);
    ~TcpConnection();

    EventLoop* getLoop() const
    {
        return m_pLoop;
    }

    const string& name() const
    {
        return m_strName;
    }

    const InetAddress& getLocalAddress() const
    {
        return m_localAddr;
    }

    const InetAddress& getPeerAddress() const
    {
        return m_peerAddr;
    }

    bool connected() const
    {
        return m_state == kConnected;
    }

    bool disConnected() const
    {
        return m_state == kDisconnected;
    }

    void setConnectionCallback(const ConnectionCallback& cb)
    {
        m_connectionCallback = cb;
    }

    void setMessageCallback(const MessageCallback& cb)
    {
        m_messageCallback = cb;
    }

    void connectEstablished();

private:
    enum StateE
    {
        kDisconnected,
        kConnecting,
        kConnected,
        kDisconnecting
    };
    void handleRead(Timestamp receiveTime);
    void setState(StateE s)
    {
        m_state = s;
    }

private:
    EventLoop * m_pLoop;
    const string m_strName;
    StateE m_state;
    std::unique_ptr<Socket> m_pSocket;
    std::unique_ptr<Channel> m_pChannel;
    const InetAddress m_localAddr;
    const InetAddress m_peerAddr;
    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
};

#endif
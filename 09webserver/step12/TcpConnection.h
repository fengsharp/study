#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <any>
#include <memory>

#include "Buffer.h"
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

    EventLoop * getLoop() const
    {
        return m_pLoop;
    }

    const string & name() const
    {
        return m_strName;
    }

    const InetAddress & getLocalAddress() const
    {
        return m_localAddr;
    }

    const InetAddress & getPeerAddress() const
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

    // void send(string&& message); // C++11
    void send(const void * message, int len);
    void send(const StringPiece & message);
    // void send(Buffer&& message); // C++11
    void send(Buffer * message); // this one will swap data
    void shutdown(); // NOT thread safe, no simultaneous calling
    void setTcpNoDelay(bool on);

    Buffer * inputBuffer()
    {
        return &m_inputBuffer;
    }

    Buffer * outputBuffer()
    {
        return &m_outputBuffer;
    }

    void setConnectionCallback(const ConnectionCallback & cb)
    {
        m_connectionCallback = cb;
    }

    void setMessageCallback(const MessageCallback & cb)
    {
        m_messageCallback = cb;
    }

    void setCloseCallback(const CloseCallback & cb)
    {
        m_closeCallback = cb;
    }

    void connectEstablished();
    void connectDestroyed();

private:
    enum StateE
    {
        kDisconnected,
        kConnecting,
        kConnected,
        kDisconnecting
    };
    void handleRead(Timestamp receiveTime);
    void handleClose();
    void handleError();
    void sendInLoop(const StringPiece & message);
    void sendInLoop(const void * message, size_t len);
    void shutdownInLoop();

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
    CloseCallback m_closeCallback;
    Buffer m_inputBuffer;
    Buffer m_outputBuffer;
};

#endif
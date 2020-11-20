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

    bool getTcpInfo(struct tcp_info*) const;
    string getTcpInfoString() const;

    // void send(string&& message); // C++11
    void send(const void * message, int len);
    void send(const StringPiece & message);
    // void send(Buffer&& message); // C++11
    void send(Buffer * message); // this one will swap data
    void shutdown(); // NOT thread safe, no simultaneous calling
    void forceClose();
    void forceCloseWithDelay(double seconds);
    void setTcpNoDelay(bool on);

    void startRead();
    void stopRead();

    bool isReading() const
    {
        return m_bReading;
    }

    Buffer * inputBuffer()
    {
        return &m_inputBuffer;
    }

    Buffer * outputBuffer()
    {
        return &m_outputBuffer;
    }

    void setContext(const std::any& context)
    {
        m_context = context;
    }

    const std::any & getContext() const
    {
        return m_context;
    }

    std::any * getMutableContext()
    {
        return &m_context;
    }

    void setHighWaterMarkCallback(const HighWaterMarkCallback & cb, size_t highWaterMark)
    {
        m_highWaterMarkCallback = cb;
        m_highWatcherMark = highWaterMark;
    }

    void setWriteCompleteCallback(const WriteCompleteCallback & cb)
    {
        m_writeCompleteCallback = cb;
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
    void handleWrite();
    void handleClose();
    void handleError();
    void sendInLoop(const StringPiece & message);
    void sendInLoop(const void * message, size_t len);
    void shutdownInLoop();
    void forceCloseInLoop();
    void setState(StateE s)
    {
        m_state = s;
    }
    const char * stateToString() const;
    void startReadInLoop();
    void stopReadInLoop();

private:
    EventLoop * m_pLoop;
    const string m_strName;
    StateE m_state;
    bool m_bReading;
    std::unique_ptr<Socket> m_pSocket;
    std::unique_ptr<Channel> m_pChannel;
    const InetAddress m_localAddr;
    const InetAddress m_peerAddr;
    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
    WriteCompleteCallback m_writeCompleteCallback;
    HighWaterMarkCallback m_highWaterMarkCallback;
    CloseCallback m_closeCallback;
    size_t m_highWatcherMark;
    Buffer m_inputBuffer;
    Buffer m_outputBuffer;
    std::any m_context;
};

#endif
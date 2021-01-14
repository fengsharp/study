#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>

#include "NonCopyable.h"
#include "Timestamp.h"

class EventLoop;
class Poller;

typedef std::function<void(Timestamp)> ChannelReadCallback;
typedef std::function<void()> ChannelCallback;

enum class ChannelStatus
{
    NEW,
    ADDED,
    DELETED
};

class Channel : private NonCopyable
{
public:
    Channel(EventLoop * pLoop, int fd);

    void setReadCallback(const ChannelReadCallback & cb)
    {
        m_readCallback = cb;
    }

    void setWriteCallback(const ChannelCallback & cb)
    {
        m_writeCallback = cb;
    }

    void setErrorCallback(const ChannelCallback & cb)
    {
        m_errorCallback = cb;
    }

    void handle(Timestamp receivedTime);

    void enableReading();
    void enableWriting();
    void disableWriting();
    void disableAll();
    bool hasEvents();

    int fd() const
    {
        return m_fd;
    }

private:
    friend class Poller;
    void setStatus(ChannelStatus status)
    {
        m_status = status;
    }

    ChannelStatus status() const
    {
        return m_status;
    }

    int events() const
    {
        return m_events;
    }

    void setReceivedEvents(int events)
    {
        m_receivedEvents = events;
    }

    void update();

private:
    EventLoop * m_pLoop;
    int m_fd;

    int m_events;
    int m_receivedEvents;

    ChannelStatus m_status;

    ChannelReadCallback m_readCallback;
    ChannelCallback m_writeCallback;
    ChannelCallback m_errorCallback;
};

#endif
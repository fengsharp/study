#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>

#include "NonCopyable.h"
#include "Timestamp.h"

enum class ChannelPollerStatus
{
    NEW,
    ADDED,
    DELETED
};

class EventLoop;
class Channel : private NonCopyable
{
private:
    friend class EventLoop;
    friend class Poller;

public:
    typedef std::function<void(Timestamp)> EventReadCallback;
    typedef std::function<void()> EventCallback;

    Channel(EventLoop * pLoop, int fd);

    void setReceiveEvents(int events)
    {
        m_receiveEvents = events;
    }

    int fd() const
    {
        return m_fd;
    }

    void setReadCallback(const EventReadCallback & cb)
    {
        m_readCallback = cb;
    }

    void setWriteCallback(const EventCallback & cb)
    {
        m_writeCallback = cb;
    }

    void setErrorCallback(const EventCallback & cb)
    {
        m_errorCallback = cb;
    }

    void enableRead();
    void enableWrite();
    void removeWrite();
    void enableError();
    void clearAllEvents();

private:
    bool hasNoneEvents() const;

    int events() const
    {
        return m_events;
    }

    void setPollerStatus(ChannelPollerStatus status)
    {
        m_pollerStatus = status;
    }

    ChannelPollerStatus polllerStatus() const
    {
        return m_pollerStatus;
    }

    void update();
    void handle(Timestamp receivedTime);

private:
    EventLoop * m_pLoop;
    int m_fd;

    int m_events;
    int m_receiveEvents;

    ChannelPollerStatus m_pollerStatus;

    EventReadCallback m_readCallback;
    EventCallback m_writeCallback;
    EventCallback m_errorCallback;
};

#endif
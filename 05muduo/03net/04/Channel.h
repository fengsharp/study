#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>

#include "NonCopyable.h"
#include "Timestamp.h"

class EventLoop;

enum class PollerStatus
{
    NEW,
    ADDED,
    DELETED
};

class Channel
{
public:
    typedef std::function<void(Timestamp)> EventReadCallback;
    typedef std::function<void()> EventCallback;

    Channel(EventLoop * pLoop, int fd);

    int fd() const
    {
        return m_fd;
    }

    void enableReading();
    void enableWriting();
    void removeWriting();
    void handle(Timestamp receiveTime);

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

private:
    void update();
private:
    friend class Poller;
    void setReciveEvents(int events);
    PollerStatus pollerStatus() const
    {
        return m_pollerStatus;
    }

    void setPollerStatus(PollerStatus status)
    {
        m_pollerStatus = status;
    }

    bool hasNoneEvents() const;

    int events() const
    {
        return m_events;
    }

private:
    EventLoop * m_pLoop;
    int m_fd;
    PollerStatus m_pollerStatus;

    int m_receivedEvents;
    int m_events;

    EventReadCallback m_readCallback;
    EventCallback m_writeCallback;
    EventCallback m_errorCallback;
};

#endif
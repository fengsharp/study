#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>

#include "NonCopyable.h"
#include "Timestamp.h"

class EventLoop;
class Poller;

enum class ChannelPollerStatus
{
    NEW,
    ADDED,
    DELETED,
};

class Channel : private NonCopyable
{
public:
    typedef std::function<void(Timestamp)> EventReadCallback;
    typedef std::function<void()> EventCallback;

    Channel(EventLoop * pLoop, int fd);
    int fd() const
    {
        return m_fd;
    }

    void setEventReadCallback(const EventReadCallback & cb)
    {
        m_readCallback = cb;
    }

    void setEventWriteCallback(const EventCallback & cb)
    {
        m_writeCallback = cb;
    }

    void setErrorCallback(const EventCallback & cb)
    {
        m_errorCallback = cb;
    }

    void enableReading();
    void enableWriting();
    void disableWriting();
    void disableAll();

private:
    friend class EventLoop;
    friend class Poller;

    int events() const
    {
        return m_events;
    }

    void setReceivedEvents(int events)
    {
        m_receivedEvents = events;
    }

    ChannelPollerStatus pollerStatus() const
    {
        return m_pollerStatus;
    }

    void setPollerStatus(ChannelPollerStatus status)
    {
        m_pollerStatus = status;
    }

    void clearAllEvents();
    bool hasNoneEvents() const;
    void update();

    void handleEvent(Timestamp receivedTime);
    EventLoop * ownerLoop() const
    {
        return m_pLoop;
    }
private:
    EventLoop * m_pLoop;
    int m_fd;

    int m_events;
    int m_receivedEvents;
    ChannelPollerStatus m_pollerStatus;

    EventReadCallback m_readCallback;
    EventCallback m_writeCallback;
    EventCallback m_errorCallback;
};

#endif
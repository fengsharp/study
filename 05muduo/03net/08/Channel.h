#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>

#include "NonCopyable.h"

class EventLoop;
class Poller;

enum class PollerStatus
{
    NEW,
    ADDED,
    DELETED,
};

class Channel : private NonCopyable
{
public:
    typedef std::function<void()> EventCallback;

    Channel(EventLoop * pLoop, int fd);

    int events() const
    {
        return m_events;
    }

    void setReceivedEvents(int events)
    {
        m_receivedEvents = events;
    }

    void handleEvent();
    void setReadCallback(const EventCallback & cb)
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

    bool isNoneEvents() const;

    void enableReading();
    void enableWriting();
    void disableWriting();
    void disableAll();
private:
    friend class Poller;
    PollerStatus status()
    {
        return m_status;
    }
    void setStatus(PollerStatus status)
    {
        m_status = status;
    }

    int fd() const
    {
        return m_fd;
    }

    void updateChannel();
private:
    EventLoop * m_pLoop;
    int m_fd;

    PollerStatus m_status;

    int m_events;
    int m_receivedEvents;

    EventCallback m_readCallback;
    EventCallback m_writeCallback;
    EventCallback m_errorCallback;
};

#endif
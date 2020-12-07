#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>

#include "EventLoop.h"
#include "Noncopyable.h"

class Channel : private NonCopyable
{
public:
    typedef std::function<void()> EventCallback;

    Channel(EventLoop * pLoop, int fd);

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

    int fd() const
    {
        return m_fd;
    }

    int events() const
    {
        return m_events;
    }

    void setReceiveEvents(int revt)
    {
        m_receiveEvents = revt;
    }

    bool isNoneEvent() const
    {
        return m_events == kNoneEvent;
    }

    void enableReading()
    {
        m_events |= kReadEvent;
        update();
    }

    void enableWriting()
    {
        m_events |= kWriteEvent;
        update();
    }

    void disableWritting()
    {
        m_events &= ~kWriteEvent;
        update();
    }

    void disableAll()
    {
        m_events = kNoneEvent;
        update();
    }

    PollerState pollerState()
    {
        return m_pollerState;
    }

    void setPollerState(PollerState state)
    {
        m_pollerState = state;
    }

    EventLoop * ownerLoop()
    {
        return m_pLoop;
    }

    void remove();
private:
    void update();

private:
    EventLoop * m_pLoop;
    int m_fd;

    int m_events;
    int m_receiveEvents;
    PollerState m_pollerState;

    EventCallback m_readCallback;
    EventCallback m_writeCallback;
    EventCallback m_errorCallback;

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
};

#endif
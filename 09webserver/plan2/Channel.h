#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>
#include <memory>

#include "NoneCopyable.h"
#include "Timestamp.h"

class EventLoop;

class Channel : private NoneCopyable
{
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(Timestamp)> ReadEventCallback;

    Channel(EventLoop * loop, int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);
    void setReadCallback(ReadEventCallback cb)
    {
        m_readCallback = cb;
    }
    void setWriteCallback(EventCallback cb)
    {
        m_writeCallback = cb;
    }
    void setCloseCallbak(EventCallback cb)
    {
        m_closeCallback = cb;
    }
    void setErrorCallback(EventCallback cb)
    {
        m_errorCallback = cb;
    }

    void tie(const std::shared_ptr<void> &);

    int fd() const
    {
        return m_fd;   
    }
    int events() const
    {
        return m_events;
    }
    void setReceiveEvents(int events)
    {
        m_receiveEvents = events;
    }
    bool isNoneEvents() const
    {
        return m_events == kNoneEvent;
    }

    void enableReading()
    {
        m_events |= kReadEvent;
        update();
    }
    void disableReading()
    {
        m_events &= ~kReadEvent;
        update();
    }
    void enableWriting()
    {
        m_events |= kWriteEvent;
        update();
    }
    void disableWriting()
    {
        m_events &= ~kWriteEvent;
        update();
    }
    void disableAll()
    {
        m_events = kNoneEvent;
        update();
    }
    bool isWriting() const
    {
        return m_events & kWriteEvent;
    }
    bool isReading() const
    {
        return m_events & kReadEvent;
    }

    int pollerIndex() const
    {
        return m_pollerIndex;
    }
    void setPollerIndex(int index)
    {
        m_pollerIndex = index;
    }

    string receiveEventsToString() const;
    string eventsToString() const;

    void doNotLogHup()
    {
        m_bLogHup = false;
    }

    EventLoop* ownerLoop()
    {
        return m_loop;
    }
    void remove();
    
private:
    static string eventsToString(int fd, int ev);

    void update();
    void handleEventWithGuard(Timestamp receiveTime);

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop * m_loop;
    const int m_fd;
    int m_events;
    int m_receiveEvents;
    int m_pollerIndex;
    bool m_bLogHup;

    std::weak_ptr<void> m_tie;
    bool m_bTied;
    bool m_bEventHandling;
    bool m_bAddedToLoop;
    ReadEventCallback m_readCallback;
    EventCallback m_writeCallback;
    EventCallback m_closeCallback;
    EventCallback m_errorCallback;
};

#endif
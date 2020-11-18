#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>
#include <memory>

#include "NoneCopyable.h"
#include "Timestamp.h"
#include "Types.h"

class EventLoop;

class Channel : private NoneCopyable
{
public:
    typedef std::function<void(Timestamp)> ReadEventCallbck;
    typedef std::function<void(void)> EventCallbck;

public:
    Channel(EventLoop * pLoop, int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);

    void setReadCallback(ReadEventCallbck cb)
    {
        m_readCallback = cb;
    }
    void setWriteCallbck(EventCallbck cb)
    {
        m_writeCallback = cb;
    }
    void setCloseCallbck(EventCallbck cb)
    {
        m_closeCallback = cb;
    }
    void setErrorCallbck(EventCallbck cb)
    {
        m_errorCallback = cb;
    }

    void tie(const std::shared_ptr<void>&);

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
        m_revents = revt;
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

    int pollerState() const
    {
        return m_pollerStatus;
    }
    void setPollerState(int state)
    {
        m_pollerStatus = state;
    }

    string reventsToString() const;
    string eventsToString() const;

    void doNotLogHup()
    {
        m_bLogHup = false;
    }

    EventLoop* ownerLoop()
    {
        return m_pLoop;
    }
    void remove();
private:
    static string eventsToString(int fd, int ev);
    void update();
    void handleEventWithGuard(Timestamp receiveTime);

private:
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop * m_pLoop;
    const int m_fd;
    int m_events;
    int m_revents;
    int m_pollerStatus;
    bool m_bLogHup;

    std::weak_ptr<void> m_tie;
    bool m_bTied;
    bool m_bEventHanding;
    bool m_bAddedToLoop;

    ReadEventCallbck m_readCallback;
    EventCallbck m_writeCallback;
    EventCallbck m_closeCallback;
    EventCallbck m_errorCallback;
};

#endif
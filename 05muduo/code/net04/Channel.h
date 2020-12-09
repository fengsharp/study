#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>
#include <memory>

#include "Noncopyable.h"
#include "Timestamp.h"
#include "Types.h"

enum CtlState
{
    NEW,
    ADDED,
    DELETED
};

class EventLoop;

class Channel : private NonCopyable
{
public:
    typedef std::function<void(Timestamp)> ReadEventCallback;
    typedef std::function<void()> EventCallback;

    Channel(EventLoop * pLoop, int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);

    void setReadCallback(const ReadEventCallback & cb)
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

    int events() const
    {
        return m_evnets;
    }

    void setReceiveEvents(int revt)
    {
        m_receiveEvents = revt;
    }

    bool isNoneEvents() const
    {
        return m_evnets == kNoneEvnet;
    }

    void enableReading()
    {
        m_evnets |= kReadEvent;
        update();
    }

    void disableRead()
    {
        m_evnets &= ~kReadEvent;
        update();
    }

    void enableWriting()
    {
        m_evnets |= kWriteEvent;
        update();
    }

    void disableWriting()
    {
        m_evnets &= ~kWriteEvent;
        update();
    }

    void disableAll()
    {
        m_evnets = kNoneEvnet;
        update();
    }

    CtlState ctlState() const
    {
        return m_ctlStatus;
    }

    void setCtlState(CtlState state)
    {
        m_ctlStatus = state;
    }

    void remove();
    int fd() const
    {
        return m_fd;
    }
    
    const EventLoop * ownerLoop() const
    {
        return m_pLoop;
    }
private:
    void update();

private:
    EventLoop * m_pLoop;
    const int m_fd; // socket timerfd wakeupfd filefd
    int m_evnets;
    int m_receiveEvents;
    CtlState m_ctlStatus;
    
    bool m_bEventHanding;
    bool m_bAddedToLoop;

    ReadEventCallback m_readCallback;
    EventCallback m_writeCallback;
    EventCallback m_errorCallback;

    static const int kNoneEvnet;
    static const int kReadEvent;
    static const int kWriteEvent;
};

#endif
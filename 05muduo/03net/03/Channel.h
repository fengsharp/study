#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>

#include "NonCopyable.h"
#include "Timestamp.h"

enum class PollerState
{
    NEW,
    ADDED,
    DELETED,
};

class EventLoop;
class Channel : private NonCopyable
{
public:
    typedef std::function<void(Timestamp)> ReadEventFuncType;
    typedef std::function<void()> EventFuncType;

    Channel(EventLoop * pLoop, int fd);

    PollerState pollerSate() const
    {
        return m_pollerState;
    }

    void setPollerState(PollerState state)
    {
        m_pollerState = state;
    }

    bool isNoneEvents() const;
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

    void handle(Timestamp receiveTime);
    void setReadEventFunc(const ReadEventFuncType & cb)
    {
        m_readEventCallback = cb;
    }

    void setWriteEventFunc(const EventFuncType & cb)
    {
        m_writeEventCallback = cb;
    }

    void setErrorEventFunc(const EventFuncType & cb)
    {
        m_errorEventCallback = cb;
    }

    void enableReading();
    void enableWriting();
    void disableWriting();
    void disableAll();
private:
    void update();
private:
    EventLoop * m_pLoop;
    int m_fd;

    int m_events;
    int m_receiveEvents;

    PollerState m_pollerState;

    ReadEventFuncType m_readEventCallback;
    EventFuncType m_writeEventCallback;
    EventFuncType m_errorEventCallback;
};

#endif
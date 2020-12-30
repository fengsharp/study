#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>

#include "NonCopyable.h"
#include "Timestamp.h"

class EventLoop;

class Channel : private NonCopyable
{
public:
    typedef std::function<void(Timestamp)> ReadEventCallbck;
    typedef std::function<void()> EventCallback;

    Channel(EventLoop * pLoop, int fd);

    void handleEvent(Timestamp receiveTime);
    void setReadCallback(const ReadEventCallbck & cb)
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

    bool isNoneEvents() const;
    void enableReading();

    int status() const
    {
        return m_status;
    }
    void setStatus(int idx)
    {
        m_status = idx;
    }
    EventLoop * ownerLoop()
    {
        return m_pLoop;
    }

private:
    void update();

private:
    EventLoop * m_pLoop;
    const int m_fd;
    int m_events;
    int m_receiveEvents;
    int m_status; // used by poller

    ReadEventCallbck m_readCallback;
    EventCallback m_writeCallback;
    EventCallback m_errorCallback;
};

#endif
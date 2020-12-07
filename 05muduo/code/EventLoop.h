#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <memory>
#include <vector>

#include "CurrentThread.h"
#include "Thread.h"

enum PollerState
{
    NEW = 0,
    ADDED = 1,
    DELETED = 2
};

class Channel;
class Poller;

class EventLoop : private NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void updateChannel(Channel * pChannel);

    void assertInLoopThread()
    {
        if (!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const
    {
        return m_threadId == CurrentThread::tid();
    }

private:
    void abortNotInLoopThread();

private:
    bool m_bLooping;
    bool m_bQuit;
    const pid_t m_threadId;
    std::unique_ptr<Poller> m_pPoller;
    std::vector<Channel *> m_activeChannels;
};

#endif
#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <memory>
#include <vector>

#include "CurrentThread.h"
#include "Noncopyable.h"
#include "Timestamp.h"

class Channel;
class Poller;

class EventLoop : private NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    bool hasChannel(Channel * pChannel);
    void removeChannel(Channel * pChannel);
    void updateChannel(Channel * pChannel);

    bool isInLoopThread() const
    {
        return m_threadId == CurrentThread::tid();
    }

    void assertLoopInThisThread() const
    {
        assert(!isInLoopThread());
    }

private:
    bool m_bLooping;
    bool m_bQuit;
    bool m_bEventHanding;
    pid_t m_threadId;
    Timestamp m_pollReturnTime;
    std::unique_ptr<Poller> m_pPoller;

    std::vector<Channel *> m_vecActiveChannels;
    Channel * m_pCurrentActiveChannel;
};

#endif
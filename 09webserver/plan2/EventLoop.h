#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <any>
#include <atomic>
#include <functional>
#include <memory>
#include <vector>

#include "CurrentThread.h"
#include "NoneCopyable.h"
#include "Timestamp.h"

class Channel;
class Poller;

class EventLoop : private NoneCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    Timestamp pollReturnTime() const
    {
        return m_pollReturnTime;
    }

    void updateChannel(Channel* pChannel);
    void removeChannel(Channel* pChannel);

    void assertInLoopThread()
    {
        if (!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const
    {
        return m_threadId == CurrentThread::gettid();
    }

    bool hasChannel(Channel* pChannel);

    static EventLoop * getEventLoopOfCurrentThread();

private:
    void abortNotInLoopThread();
     void printActiveChannels() const; // DEBUG
     
private:
    bool m_bLooping;
    std::atomic<bool> m_bQuit;
    bool m_bEventHanding;
    const pid_t m_threadId; // 当前对象所属线程ID
    Timestamp m_pollReturnTime;
    std::unique_ptr<Poller> m_poller;
    std::vector<Channel *> m_vecActiveChannels;
    Channel * m_pCurrentActiveChannel;
};

#endif
#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <any>
#include <atomic>
#include <functional>
#include <vector>
#include <memory>

#include "CurrentThread.h"
#include "Mutex.h"
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
        return m_pollRetrunTime;
    }

    bool hasChannel(Channel * pChannel);
    void updateChannel(Channel * pChannel); // 在Poller中添加或者更新通道
    void removeChannel(Channel * pChannel); // 从Poller中移除通道

    void assertInLoopThread();
    bool isInLoopThread();

    static EventLoop * getEventLoopOfCurrentThread();

private:
    void abortNotInLoopThread();
    void printActiveChannels() const; // DEBUG

private:
    bool m_bLooping; // atomic
    std::atomic<bool> m_bQuit;
    bool m_bEventHanding; // atomic
    const pid_t m_threadId;
    Timestamp m_pollRetrunTime;
    std::unique_ptr<Poller> m_pPoller;

    std::vector<Channel *> m_vecActiveChannels;
    Channel * m_pCurrentActiveChannel;
};

#endif
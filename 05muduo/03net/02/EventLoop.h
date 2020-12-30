#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <pthread.h>

#include <memory>
#include <vector>

#include "NonCopyable.h"

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

    bool isInLoopThread();
    void assertInLoopThread();

private:
    bool m_bLooping;
    const pid_t m_threadId;
    bool m_bQuit;
    std::unique_ptr<Poller> m_pPoller;

    std::vector<Channel *> m_vecActiveChannels;
};

#endif
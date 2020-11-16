#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <any>
#include <atomic>
#include <functional>
#include <vector>

#include "CurrentThread.h"
#include "Mutex.h"
#include "Timestamp.h"

class EventLoop : private NoneCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void assertInLoopThread();
    bool isInLoopThread();

    static EventLoop * getEventLoopOfCurrentThread();

private:
    void abortNotInLoopThread();

private:
    bool m_bLooping; // atomic
    const pid_t m_threadId;
};

#endif
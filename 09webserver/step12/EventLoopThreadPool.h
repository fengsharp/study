#ifndef EVENT_LOOP_THREAD_POOL_H_
#define EVENT_LOOP_THREAD_POOL_H_

#include <functional>
#include <memory>
#include <vector>

#include "NoneCopyable.h"
#include "Types.h"

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : private NoneCopyable
{
public:
    typedef std::function<void(EventLoop *)> ThreadInitCallback;

    EventLoopThreadPool(EventLoop * pBaseLoop, const string & nameArg);
    ~EventLoopThreadPool();

    void setThreadNum(int numThreads)
    {
        m_numThreads = numThreads;
    }
    void start(const ThreadInitCallback & cb = ThreadInitCallback());

    EventLoop * getNextLoop();

    EventLoop * getLoopForHash(size_t hashCode);

    std::vector<EventLoop *> getAllLoops();

    bool started() const
    {
        return m_bStarted;
    }

    const string & name() const
    {
        return m_strName;
    }

private:
    EventLoop * m_pBaseLoop;
    string m_strName;
    bool m_bStarted;
    int m_numThreads;
    int m_next;
    std::vector<std::unique_ptr<EventLoopThread>> m_vecThreads;
    std::vector<EventLoop *> m_vecLoops;
};

#endif
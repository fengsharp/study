#include "EventWakeQueue.h"

#include <assert.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "EventLoop.h"

static int creteEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    assert(evtfd >= 0);
    return evtfd;
}

EventWakeQueue::EventWakeQueue(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_eventfd(creteEventfd())
    , m_eventChannel(m_pLoop, m_eventfd)
    , m_mutex()
    , m_bPending(false)
{
    m_eventChannel.setEventReadCallback(std::bind(&EventWakeQueue::handleRead, this, std::placeholders::_1));
    m_eventChannel.enableReading();
}

EventWakeQueue::~EventWakeQueue()
{
    ::close(m_eventfd);
}

void EventWakeQueue::addPendingFunction(const LoopFunctor & cb)
{
    {
        MutexLockGuard lock(m_mutex);
        m_vecPendingFunctions.push_back(cb);
    }

    if (m_pLoop->isLoopInThread() == false || m_bPending)
    {
        wakeup();
    }
}

void EventWakeQueue::run()
{
    std::vector<LoopFunctor> vecPendings;
    m_bPending = true;

    {
        MutexLockGuard lock(m_mutex);
        vecPendings.swap(m_vecPendingFunctions);
    }

    for (auto & item : vecPendings)
    {
        item();
    }

    m_bPending = false;
}

void EventWakeQueue::handleRead(Timestamp receivedTime)
{
    uint64_t one = 1;
    ssize_t n = ::read(m_eventfd, &one, sizeof(one));
    assert(n == sizeof(one));
}

void EventWakeQueue::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(m_eventfd, &one, sizeof(one));
    assert(n == sizeof(one));
}

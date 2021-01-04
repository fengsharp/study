#include "PendingQueue.h"

#include <assert.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <functional>

#include "EventLoop.h"

static int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        abort();
    }
    return evtfd;
}

PendingQueue::PendingQueue(EventLoop * pLoop)
    : m_bCallPending(false)
    , m_pLoop(pLoop)
    , m_wakeupfd(::createEventfd())
    , m_channel(m_pLoop, m_wakeupfd)
    , m_mutex()
{
    m_channel.setReadCallback(std::bind(&PendingQueue::handleRead, this, std::placeholders::_1));
    m_channel.enableReading();
}

PendingQueue::~PendingQueue()
{
    ::close(m_wakeupfd);
}

void PendingQueue::addCallback(const PendingCallback & cb)
{
    {
        MutexLockGuard lock(m_mutex);
        m_vecPendingFunctors.push_back(cb);
    }

    if (!m_pLoop->isLoopInThread() || m_bCallPending)
    {
        wakeup();
    }
}

void PendingQueue::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(m_wakeupfd, &one, sizeof(one));
    assert(n == sizeof(one));
}

void PendingQueue::handleRead(Timestamp receivedTime)
{
    uint64_t one = 1;
    ssize_t n = ::read(m_wakeupfd, &one, sizeof(one));
    assert(n == sizeof(one));
}

void PendingQueue::callPendingFunctiors()
{
    m_pLoop->assertLoopInThread();

    std::vector<PendingCallback> functors;
    m_bCallPending = true;
    {
        MutexLockGuard lock(m_mutex);
        functors.swap(m_vecPendingFunctors);
    }

    for (auto & item : functors)
    {
        item();
    }

    m_bCallPending = false;
}
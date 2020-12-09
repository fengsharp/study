#include "WeakupQueue.h"

#include <assert.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "EventLoop.h"
#include "Logging.h"
#include "Timestamp.h"

static int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG_SYSERR << "failed in event";
    }
    return evtfd;
}

WeakupQueue::WeakupQueue(EventLoop * pLoop)
    : m_pLoop(pLoop)
    , m_bCallingPendingFunctors(false)
    , m_wakeupFd(createEventfd())
    , m_weakupChannel(m_pLoop, m_wakeupFd)
    , m_mutex()
{
    m_weakupChannel.setReadCallback(std::bind(&WeakupQueue::handleRead, this, std::placeholders::_1));
    m_weakupChannel.enableReading();
}

WeakupQueue::~WeakupQueue()
{
    ::close(m_wakeupFd);
}

void WeakupQueue::handleRead(Timestamp)
{
    uint64_t one = 1;
    ssize_t n = read(m_wakeupFd, &one, sizeof(one));
    if (n != sizeof(one))
    {
        LOG_ERROR << "handleread reads bytes: " << n;
    }
}

void WeakupQueue::doPendingFunctors()
{
    std::vector<Functor> functors;
    m_bCallingPendingFunctors = true;

    {
        MutexLockGuard lock(m_mutex);
        functors.swap(m_vecPendingFunctors);
    }

    for (Functor & item : functors)
    {
        item();
    }
    
    m_bCallingPendingFunctors = false;
}

void WeakupQueue::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(m_wakeupFd, &one, sizeof(one));
    if (n != sizeof(one))
    {
        LOG_ERROR << "wakeup write bytes: " << n;
    }
}

void WeakupQueue::push(const Functor & cb)
{
    {
        MutexLockGuard lock(m_mutex);
        m_vecPendingFunctors.push_back(cb);
    }

    if (m_pLoop->isInLoopThread() == false || m_bCallingPendingFunctors)
    {
        wakeup();
    }
}
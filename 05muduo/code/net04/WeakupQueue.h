#ifndef WEAKUP_QUEUE_H_
#define WEAKUP_QUEUE_H_

#include <memory>
#include <vector>

#include "Callbacks.h"
#include "Channel.h"
#include "Mutex.h"
#include "Noncopyable.h"

class EventLoop;
class Timestamp;

class WeakupQueue : private NonCopyable
{
public:
    explicit WeakupQueue(EventLoop * pLoop);
    ~WeakupQueue();
    
    void doPendingFunctors();
    void wakeup();
    void push(const Functor & cb);
    
private:
    void handleRead(Timestamp);
private:
    EventLoop * m_pLoop;
    bool m_bCallingPendingFunctors;
    int m_wakeupFd;
    Channel m_weakupChannel;
    MutexLock m_mutex;
    std::vector<Functor> m_vecPendingFunctors GUARDED_BY(m_mutex);
};

#endif
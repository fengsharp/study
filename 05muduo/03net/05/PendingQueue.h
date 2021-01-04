#ifndef PENDING_QUEUE_H_
#define PENDING_QUEUE_H_

#include <vector>

#include "NonCopyable.h"
#include "Callbacks.h"
#include "Mutex.h"
#include "Channel.h"
#include "Timestamp.h"

class EventLoop;
class PendingQueue : private NonCopyable
{
public:
    explicit PendingQueue(EventLoop * pLoop);
    ~PendingQueue();

    void addCallback(const PendingCallback & cb);
    void wakeup();
    void callPendingFunctiors();
private:
    void handleRead(Timestamp receivedTime);
private:
    bool m_bCallPending;

    EventLoop * m_pLoop;
    int m_wakeupfd;
    Channel m_channel;

    MutexLock m_mutex;
    std::vector<PendingCallback> m_vecPendingFunctors GUARDED_BY(m_mutex);
};

#endif
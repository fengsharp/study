#ifndef EVENT_WAKE_QUEUE_H_
#define EVENT_WAKE_QUEUE_H_

#include <vector>

#include "Mutex.h"
#include "NonCopyable.h"
#include "Callbacks.h"
#include "Channel.h"

class EventLoop;
class EventWakeQueue : private NonCopyable
{
public:
    explicit EventWakeQueue(EventLoop * pLoop);
    ~EventWakeQueue();

    void addPendingFunction(const LoopFunctor & cb);

    void run();

    void wakeup();
private:
    void handleRead(Timestamp receivedTime);
private:
    EventLoop * m_pLoop;
    int m_eventfd;
    Channel m_eventChannel;

    MutexLock m_mutex;
    std::vector<LoopFunctor> m_vecPendingFunctions GUARDED_BY(m_mutex);
    bool m_bPending;
};

#endif
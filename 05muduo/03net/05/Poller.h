#ifndef POLLER_H_
#define POLLER_H_

#include <vector>

#include "NonCopyable.h"
#include "Timestamp.h"

class EventLoop;
class Channel;
struct epoll_event;

class Poller
{
public:
    explicit Poller(EventLoop * pLoop);
    ~Poller();

    Timestamp poll(int timeoutMs, std::vector<Channel*> * pOutActivityChannels);

    // add, delete, or update events
    void updateChannel(Channel * pChannel);
    void removeChannel(Channel * pChannel);
private:
    void updateChannel(int op, Channel * pChannel);
private:
    EventLoop * m_pLoop;
    int m_epollfd;
    std::vector<struct epoll_event> m_vecTmpActivityEvents;
};

#endif
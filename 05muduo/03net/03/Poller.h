#ifndef POLLER_H_
#define POLLER_H_

#include <vector>
/*
epollfd = epoll_create1(EPOLL_CLOEXEC);

struct epoll_event event;
event.data.fd = listenfd;
event.events = EPOLLIN;
epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);

nready = epoll_wait(epollfd, &*events.begin(), static_cast<int>(events.size()), -1);

epoll_ctl(epollfd, EPOLL_CTL_DEL, connfd, &event);
*/

#include "NonCopyable.h"
#include "Timestamp.h"

class Channel;
class EventLoop;

struct epoll_event;

class Poller : private NonCopyable
{
public:
    Poller(EventLoop * pLoop);
    ~Poller();

    Timestamp poll(int timeoutMs, std::vector<Channel *> * pOutActiveChanels);

    void updateChannel(Channel * pChannel);
    void removeChannel(Channel * pChannel);

private:
    void update(int operation, Channel * pChannel);
    void assertInLoopThread();
private:
    EventLoop * m_pLoop;

    int m_epollfd;

    std::vector<struct epoll_event> m_vecTmpReceiveEvents;
};

#endif
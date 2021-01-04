#include <stdio.h>

#include <assert.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <functional>

#include "Channel.h"
#include "EventLoop.h"
#include "Timestamp.h"
#include "Types.h"
/*
int timerfd_create(int clockid, int flags);
int timerfd_settime(int fd, int flags,
                           const struct itimerspec *new_value,
                           struct itimerspec *old_value);

*/

EventLoop * g_loop = nullptr;
int timerfd = 0;

void timerCallback()
{
    puts("timerCallback");

    g_loop->stop();
}

int main()
{
    EventLoop pLoop;
    g_loop = &pLoop;
    pLoop.runAfter(2.0, std::bind(&timerCallback));

    pLoop.loop();

    puts("=== ok. ===");

    return 0;
}
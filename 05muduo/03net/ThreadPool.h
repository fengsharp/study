#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <list>
#include <vector>
#include <functional>
#include <memory>

#include "Thread.h"
#include "BlockingQueue.h"

class ThreadPool : private NonCopyable
{
public:
    typedef std::function<void()> Task;

    ThreadPool();
    ~ThreadPool();

    void start(int fixedThreadNum);
    void stop();

    void addTask(const Task & task);
private:
    void threadPoolFunc();
private:
    bool m_bRunning;
    std::vector<std::unique_ptr<Thread>> m_vecThreads;
    BlockingQueue<Task> m_taskQueue;
};

#endif
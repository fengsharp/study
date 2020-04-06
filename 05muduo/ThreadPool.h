#ifndef MUDUO_BASE_THREADPOOL_H
#define MUDUO_BASE_THREADPOOL_H

#include <deque>
#include <vector>
#include <string>

#include "Thread.h"
#include "MutexLock.h"
#include "Condition.h"
#include "noncopyable.h"

class ThreadPool : noncopyable
{
public:
    typedef std::function<void ()> TaskFunc;
    explicit ThreadPool(const std::string& nameArg = std::string("ThreadPool"));
    ~ThreadPool();

    void setMaxQueueSize(int maxSize) { m_taskQueueMaxSize = maxSize; }
    void setThreadInitCallback(const TaskFunc& cb) { m_threadInitCallback = cb; }

    void start(int numThreads);
    void stop();

    const std::string& name() const { return m_name; }

    size_t queueSize() const;
    void run(TaskFunc f);

private:
    bool isFull() const;
    void runInThread();
    TaskFunc take();

private:
    mutable MutexLock m_mutex;
    Condition m_notEmptyCond;
    Condition m_notFullCond;
    std::string m_name;
    TaskFunc m_threadInitCallback;

    std::vector<std::unique_ptr<Thread>> m_rgThreads;

    std::deque<TaskFunc> m_taskQueue;
    size_t m_taskQueueMaxSize;

    bool m_bRunning;
};


#endif
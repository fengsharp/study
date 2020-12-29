#include "ThreadPool.h"

#include <assert.h>

ThreadPool::ThreadPool()
    : m_bRunning(false)
{
}

ThreadPool::~ThreadPool()
{
    if (m_bRunning)
    {
        stop();
    }
}

void ThreadPool::start(int fixedThreadNum)
{
    assert(m_bRunning == false);
    m_bRunning = true;
    m_vecThreads.reserve(fixedThreadNum);
    for (int i = 0; i < fixedThreadNum; i++)
    {
        char nameBuf[32] = { 0 };
        snprintf(nameBuf, sizeof(nameBuf) - 1, "ThreadPool-%d", i);
        std::unique_ptr<Thread> pThread = std::make_unique<Thread>(std::bind(&ThreadPool::threadPoolFunc, this), nameBuf);
        m_vecThreads.push_back(std::move(pThread));
        m_vecThreads.back()->start();
    }
}

void ThreadPool::threadPoolFunc()
{
    while (m_bRunning)
    {
        Task task = m_taskQueue.take();
        if (task)
        {
            task();
        }
    }
}

void ThreadPool::stop()
{
    assert(m_bRunning == true);
    m_bRunning = false;

    for (size_t i = 0; i < m_vecThreads.size(); i++)
    {
        addTask(NULL);
    }

    for (auto & item : m_vecThreads)
    {
        item->join();
    }
}

void ThreadPool::addTask(const Task & task)
{
    m_taskQueue.put(task);
}
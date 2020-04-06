#include "ThreadPool.h"

#include <assert.h>
#include <stdio.h>

#include "Exception.h"

ThreadPool::ThreadPool(const std::string& nameArg)
  : m_mutex(),
    m_notEmptyCond(m_mutex), m_notFullCond(m_mutex),
    m_name(nameArg),
    m_taskQueueMaxSize(0),
    m_bRunning(false)
{
}

ThreadPool::~ThreadPool()
{
    if (m_bRunning)
    {
        stop();
    }
}

void ThreadPool::start(int numThreads)
{
    assert(m_rgThreads.empty());
    m_bRunning = true;
    m_rgThreads.reserve(numThreads);
    for (int i = 0; i < numThreads; ++i)
    {
        char id[32];
        snprintf(id, sizeof id, "%d", i+1);
        m_rgThreads.emplace_back(new Thread(std::bind(&ThreadPool::runInThread, this), m_name+id));
        m_rgThreads[i]->start();
    }

    if (numThreads == 0 && m_threadInitCallback)
    {
        m_threadInitCallback();
    }
}

void ThreadPool::stop()
{
    {
        MutexLockGuard lock(m_mutex);
        m_bRunning = false;
        m_notEmptyCond.notifyAll();
    }

    for (auto& thr : m_rgThreads)
    {
        thr->join();
    }
}

size_t ThreadPool::queueSize() const
{
    MutexLockGuard lock(m_mutex);
    return m_taskQueue.size();
}

void ThreadPool::run(TaskFunc task)
{
    if (m_rgThreads.empty())
    {
        task();
    }
    else
    {
        MutexLockGuard lock(m_mutex);
        while (isFull())
        {
            m_notFullCond.wait();
        }
        assert(!isFull());

        m_taskQueue.push_back(std::move(task));
        m_notEmptyCond.notify();
    }
}

ThreadPool::TaskFunc ThreadPool::take()
{
    MutexLockGuard lock(m_mutex);
    // always use a while-loop, due to spurious wakeup
    while (m_taskQueue.empty() && m_bRunning)
    {
        m_notEmptyCond.wait();
    }

    TaskFunc task;
    if (!m_taskQueue.empty())
    {
        task = m_taskQueue.front();
        m_taskQueue.pop_front();
        if (m_taskQueueMaxSize > 0)
        {
            m_notFullCond.notify();
        }
    }
    return task;
}

bool ThreadPool::isFull() const
{
    m_mutex.assertLocked();
    return m_taskQueueMaxSize > 0 && m_taskQueue.size() >= m_taskQueueMaxSize;
}

void ThreadPool::runInThread()
{
    try
    {
        if (m_threadInitCallback)
        {
            m_threadInitCallback();
        }

        while (m_bRunning)
        {
            TaskFunc task(take());
            if (task)
            {
                task();
            }
        }
    }
    catch (const Exception& ex)
    {
        fprintf(stderr, "exception caught in ThreadPool %s\n", m_name.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
        abort();
    }
    catch (const std::exception& ex)
    {
        fprintf(stderr, "exception caught in ThreadPool %s\n", m_name.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }
    catch (...)
    {
        fprintf(stderr, "unknown exception caught in ThreadPool %s\n", m_name.c_str());
        throw; // rethrow
    }
}

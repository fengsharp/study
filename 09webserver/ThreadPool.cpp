#include "ThreadPool.h"

#include <assert.h>
#include <stdio.h>

#include "Exception.h"

ThreadPool::ThreadPool(const string & nameArgs /* = string("ThreadPool")*/)
    : m_mutex()
    , m_notEmpty(m_mutex)
    , m_notFull(m_mutex)
    , m_strName(nameArgs)
    , m_maxQueueSize(0)
    , m_isRunning(false)
{
}

ThreadPool::~ThreadPool()
{
    if (m_isRunning)
    {
        stop();
    }
}

void ThreadPool::start(int numThreads)
{
    assert(m_vecThreads.empty());
    m_isRunning = true;
    m_vecThreads.reserve(numThreads);
    for (int i = 0; i < numThreads; ++i)
    {
        char id[32] = { 0 };
        snprintf(id, sizeof(id) - 1, "%d", i + 1);
        m_vecThreads.emplace_back(new Thread(std::bind(&ThreadPool::runInThread, this), m_strName + id));
        m_vecThreads[i]->start();
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
        m_isRunning = false;
        m_notEmpty.notifyAll();
    }

    for (auto & thr : m_vecThreads)
    {
        thr->join();
    }
}

size_t ThreadPool::queueSize() const
{
    MutexLockGuard lock(m_mutex);
    return m_queue.size();
}

void ThreadPool::run(std::function<void()> task)
{
    if (m_vecThreads.empty())
    {
        task();
    }
    else
    {
        MutexLockGuard lock(m_mutex);
        while (isFull())
        {
            m_notFull.wait();
        }
        assert(!isFull());

        m_queue.push_back(std::move(task));
        m_notEmpty.notify();
    }
}

std::function<void()> ThreadPool::take()
{
    MutexLockGuard lock(m_mutex);
    while (m_queue.empty() && m_isRunning)
    {
        m_notEmpty.wait();
    }

    std::function<void()> task;
    if (!m_queue.empty())
    {
        task = m_queue.front();
        m_queue.pop_front();
        if (m_maxQueueSize > 0)
        {
            m_notFull.notify();
        }
    }
    return task;
}

bool ThreadPool::isFull() const
{
    m_mutex.assertLocked();
    return m_maxQueueSize > 0 && m_queue.size() >= m_maxQueueSize;
}

void ThreadPool::runInThread()
{
    try
    {
        if (m_threadInitCallback)
        {
            m_threadInitCallback();
        }

        while (m_isRunning)
        {
            std::function<void()> task = take();
            if (task)
            {
                task();
            }
        }
    }
    catch (const Exception & ex)
    {
        fprintf(stderr, "exception caught in ThreadPool %s\n", m_strName.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
        abort();
    }
    catch (const std::exception & ex)
    {
        fprintf(stderr, "exception caught in ThreadPool %s\n", m_strName.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }
    catch (...)
    {
        fprintf(stderr, "unknown exception caught in ThreadPool %s\n", m_strName.c_str());
        throw; // rethrow
    }
}
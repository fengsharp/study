#include <stdio.h>

#include <memory>
#include <string>
#include <vector>

#include "BlockingQueue.h"
#include "CountDownLatch.h"
#include "Thread.h"

/*
已经接收了数据（FuncName, params)
放入任务队列，线程池抽取数据，访问数据库，返回结果后，放入缓冲区，并调用TcpConnection.write
*/

class MyConn : private NonCopyable
{
public:
    MyConn(int id)
        : m_id(id)
    {
    }

    void write(const std::string & buffer)
    {
        printf("conn:%d respone:%s\n", m_id, buffer.data());
    }

private:
    int m_id;
};

class TaskData
{
public:
    TaskData(const std::string & readBuffer, const std::weak_ptr<MyConn> & pConn)
        : m_readBuffer(readBuffer)
        , m_pConn(pConn)
    {
    }

    const std::string & getReadBuffer() const
    {
        return m_readBuffer;
    }

    std::shared_ptr<MyConn> getConn() const
    {
        return m_pConn.lock();
    }
private:
    std::string m_readBuffer;
    std::weak_ptr<MyConn> m_pConn;
};

class LoginSimulation : private NonCopyable
{
public:
    LoginSimulation(int thrNum)
        : m_latch(thrNum)
    {
        for (int i = 0; i < thrNum; i++)
        {
            m_vecThreads.emplace_back(new Thread(std::bind(&LoginSimulation::threadFunc, this, i), "Thread" + std::to_string(i)));
        }
        
        for (auto & item : m_vecThreads)
        {
            item->start();
        }
    }

    void run(int connNum)
    {
        m_latch.wait();

        for (int i = 0; i < connNum; i++)
        {
            m_vecSimulationConns.push_back(std::make_shared<MyConn>(i));
            TaskData data("login-" + std::to_string(i) + "-login.", m_vecSimulationConns.back());
            m_taskQueue.put(data);
        }
        
    }

    void joinAll()
    {
        int num = m_vecThreads.size();
        for (int i = 0; i < num; i++)
        {
            TaskData data("stop", std::weak_ptr<MyConn>());
            m_taskQueue.put(data);
        }

        for (auto & item : m_vecThreads)
        {
            item->join();
        }
    }
private:
    void threadFunc(int index)
    {
        m_latch.countDown();

        bool running = true;
        while (running)
        {
            TaskData task = m_taskQueue.take();
            if (task.getReadBuffer() == "stop")
            {
                running = false;
            }
            else
            {
                {
                    // query from database
                    CurrentThread::sleepUsec(300);
                }

                std::shared_ptr<MyConn> pConn = task.getConn();
                if (pConn == NULL)
                {
                    printf("conn have closed.");
                }
                else
                {
                    std::string writeContent = "[ thread-" + std::to_string(index) + "- " + task.getReadBuffer() + " response ok. ]";
                    pConn->write(writeContent);
                }
            }
        }
    }

private:
    BlockingQueue<TaskData> m_taskQueue;

    CountDownLatch m_latch;
    std::vector<std::unique_ptr<Thread>> m_vecThreads;

    std::vector<std::shared_ptr<MyConn>> m_vecSimulationConns;
};

int main()
{
    LoginSimulation test(5);
    test.run(15);
    test.joinAll();

    return 0;
}
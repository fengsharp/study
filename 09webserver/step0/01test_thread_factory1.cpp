#include <stdio.h>

#include <memory>
#include <string>
#include <unordered_map>

#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "CurrentThread.h"
#include "NoneCopyable.h"
#include "Thread.h"

void sleepMs(int ms)
{
    usleep(ms * 1000);
}

class Stock : private NoneCopyable
{
public:
    explicit Stock(const std::string & name)
        : m_strName(name)
    {
        printf("%s: Stock[%p] ctor %s\n", CurrentThread::name(), this, m_strName.c_str());
    }

    ~Stock()
    {
        printf("%s: ~Stock[%p] dtor %s\n", CurrentThread::name(), this, m_strName.data());
    }

    const std::string & key() const
    {
        return m_strName;
    }

private:
    std::string m_strName;
};

class StockFactory : private NoneCopyable
{
public:
    std::shared_ptr<Stock> get(const std::string & key)
    {
        std::shared_ptr<Stock> pStock;
        MutexLockGuard lock(m_mutex);
        std::weak_ptr<Stock> & wkStock = m_mapStacks[key];
        pStock = wkStock.lock();
        if (pStock == nullptr)
        {
            pStock.reset(new Stock(key), [this](Stock * stock) { deleteStock(stock); });
            wkStock = pStock;
        }
        return pStock;
    }

private:
    void deleteStock(Stock * stock)
    {
        printf("%s: deleteStock[%p]\n", CurrentThread::name(), stock);
        if (stock)
        {
            sleepMs(500);
            if (m_mutex.isLockedByThisThread())
            {
                printf("WARNING: mutex_ is already locked by this thread, deadlock will happen. %s\n", CurrentThread::name());
            }
            MutexLockGuard lock(m_mutex);
            auto it = m_mapStacks.find(stock->key());
            assert(it != m_mapStacks.end());
#ifndef REPRODUCE_BUG
            if (auto x = it->second.lock())
            {
                assert(stock != x.get());
                printf("%s use_count = %ld\n", CurrentThread::name(), x.use_count());
                sleepMs(500);
                printf("%s use_count = %ld\n", CurrentThread::name(), x.use_count());
            }
            else
            {
                m_mapStacks.erase(it);
            }
#else
            if (it->second.expired())
            {
                m_mapStacks.erase(it);
            }
            else
            {
                printf("%s: %s is not expired\n", CurrentThread::name(), stock->key().c_str());
            }
#endif
        }
        delete stock; // sorry, I lied
        printf("%s delete finish.\n", CurrentThread::name());
    }

private:
    mutable MutexLock m_mutex;
    std::unordered_map<std::string, std::weak_ptr<Stock>> m_mapStacks;
};

void threadB(StockFactory * pFactory)
{
    sleepMs(250);
    auto stock = pFactory->get("MS");
    printf("%s: stockB %p\n", CurrentThread::name(), stock.get());

    sleepMs(500);
    printf("%s: stockB destructs\n", CurrentThread::name());
}

int main()
{
    StockFactory factory;
    Thread thr([&factory] {
        threadB(&factory);
    },
               "thrB");
    thr.start();

    {
        std::shared_ptr<Stock> stock = factory.get("MS");
        printf("%s: stock %p\n", CurrentThread::name(), stock.get());
    }

    thr.join();

    return 0;
}
#include <assert.h>
#include <stdio.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Mutex.h"
#include "NoneCopyable.h"

class Stock
{
public:
    Stock(const std::string & name)
        : m_strName(name)
    {
        printf("Stock() %p %s\n", this, m_strName.data());
    }

    ~Stock()
    {
        printf("~Stock() %p %s\n", this, m_strName.data());
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
    StockFactory()
    {
        printf("--- StockFactory()\n");
    }

    ~StockFactory()
    {
        printf("--- ~StockFactory()\n");
    }

public:
    std::shared_ptr<Stock> get(const std::string & key)
    {
        MutexLockGuard guard(m_mutex);

        std::weak_ptr<Stock> & pWkStock = m_mapStocks[key];
        std::shared_ptr<Stock> pStock = pWkStock.lock();
        if (pStock == nullptr)
        {
            pStock.reset(new Stock(key), std::bind(&StockFactory::deleteStock, this, std::placeholders::_1));
            pWkStock = pStock;
        }
        return pStock;
    }

    void printInfo() const
    {
        MutexLockGuard lock(m_mutex);
        printf("count: %lu\n", m_mapStocks.size());
    }

private:
    void deleteStock(Stock * pStock)
    {
        printf("deleteStock %p\n", pStock);
        if (pStock != nullptr)
        {
            MutexLockGuard lock(m_mutex);
            m_mapStocks.erase(pStock->key());
        }
        delete pStock;
    }

private:
    mutable MutexLock m_mutex;
    std::map<std::string, std::weak_ptr<Stock>> m_mapStocks GUARDED_BY(m_mutex);
};

void test_normal()
{
    StockFactory sf;
    {
        std::shared_ptr<Stock> s1 = sf.get("stock3");
    }

    sf.printInfo();
}

void test_bug()
{
    // s1生命周期比StockFactory长
    std::shared_ptr<Stock> s1;
    {
        StockFactory sf;
        {
            s1 = sf.get("stock3");
        }

        sf.printInfo();
    }
    // 此时s1要析构，回调绑定的方法时，StockFactory对象sf已经不存在，指针地址空悬，core dump
}

int main()
{
    // test_normal();
    test_bug();

    puts("=== over! ===");

    return 0;
}

/*
--- StockFactory()
Stock() 0x6d7080 stock3
count: 1
--- ~StockFactory()
deleteStock 0x6d7080
~Stock() 0x6d7080 stock3
*/
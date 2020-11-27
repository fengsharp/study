#include <assert.h>
#include <stdio.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Mutex.h"
#include "NoneCopyable.h"

class Stock : private NoneCopyable
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

class StockFactory : private NoneCopyable, public std::enable_shared_from_this<StockFactory>
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
            pStock.reset(new Stock(key), std::bind(StockFactory::weakDeleteCallback,
                                                   std::weak_ptr<StockFactory>(shared_from_this()), 
                                                   std::placeholders::_1));
            pWkStock = pStock;
        }
        return pStock;
    }

    void printInfo() const
    {
        MutexLockGuard lock(m_mutex);
        printf("StockFactory map count: %lu\n", m_mapStocks.size());
    }

private:
    static void weakDeleteCallback(const std::weak_ptr<StockFactory> & pWeakStockFactory, Stock * pStock)
    {
        printf("weakDeleteCallback stock %p\n", pStock);
        std::shared_ptr<StockFactory> pStockFactory = pWeakStockFactory.lock();
        if (pStockFactory != nullptr)
        {
            pStockFactory->removeStock(pStock);
        }
        else
        {
            printf("stock factory is null\n");
        }
        delete pStock;
    }

    void removeStock(Stock * pStock)
    {
        printf("removeStock %p\n", pStock);
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

void test_bug()
{
    // s1生命周期比StockFactory长
    std::shared_ptr<Stock> s1;
    {
        std::shared_ptr<StockFactory> pSf = std::make_shared<StockFactory>();
        {
            s1 = pSf->get("stock5");
        }

        pSf->printInfo(); // StockFactory对象引用基数为1，退出局部后，析构
        printf("--- now StockFactory use_count: %lu\n", pSf.use_count());
    }
    // 此时s1要析构，回调绑定的方法时，StockFactory对象提升为空
}

int main()
{
    test_bug();

    puts("=== over! ===");

    return 0;
}
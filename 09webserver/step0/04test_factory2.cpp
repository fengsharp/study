#include <assert.h>
#include <stdio.h>

#include <map>
#include <memory>
#include <string>

#include "NoneCopyable.h"
#include "Mutex.h"

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
    std::shared_ptr<Stock> get(const std::string& key)
    {
        MutexLockGuard guard(m_mutex);

        std::weak_ptr<Stock>& pWkStock = m_mapStocks[key];
        std::shared_ptr<Stock> pStock = pWkStock.lock();
        if (pStock == nullptr)
        {
            pStock = std::make_shared<Stock>(key);
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
    mutable MutexLock m_mutex;
    std::map<std::string, std::weak_ptr<Stock>> m_mapStocks GUARDED_BY(m_mutex);
};

int main()
{
    StockFactory sf;
    {
        std::shared_ptr<Stock> s1 = sf.get("stock2");
    }

    sf.printInfo();
    
    puts("=== over! ===");

    return 0;
}
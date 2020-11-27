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

        std::shared_ptr<Stock>& pStock = m_mapStocks[key];
        if (pStock == nullptr)
        {
            m_mapStocks[key] = std::make_shared<Stock>(key);
        }
        return pStock;
    }

private:
    MutexLock m_mutex;
    std::map<std::string, std::shared_ptr<Stock>> m_mapStocks GUARDED_BY(m_mutex);
};

int main()
{
    StockFactory sf;
    {
        std::shared_ptr<Stock> s1 = sf.get("stock1");
    }

    puts("=== over! ===");

    return 0;
}
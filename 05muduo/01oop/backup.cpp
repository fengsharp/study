#include <assert.h>
#include <stdio.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

class Stock
{
public:
    explicit Stock(const std::string & name)
        : m_strName(name)
    {
        printf(" stock %p %s\n", this, m_strName.data());
    }

    ~Stock()
    {
        printf(" ~stock %p %s\n", this, m_strName.data());
    }

    const std::string & getName() const
    {
        return m_strName;
    }

private:
    std::string m_strName;
};

class StockFactory : public std::enable_shared_from_this<StockFactory>
{
public:
    StockFactory()
    {
        printf("StockFactory() %p\n", this);
    }
    ~StockFactory()
    {
        printf("~StockFacotry %p, size: %lu\n", this, m_mapStocks.size());
    }
    std::shared_ptr<Stock> get(const std::string & name)
    {
        std::shared_ptr<Stock> pResult;
        std::weak_ptr<Stock> & pFindStock = m_mapStocks[name];
        pResult = pFindStock.lock();
        if (pResult == NULL)
        {
            pResult.reset(new Stock(name), std::bind(StockFactory::deleteStock, std::weak_ptr<StockFactory>(shared_from_this()), std::placeholders::_1));
            pFindStock = pResult;
        }
        return pResult;
    }

private:
    static void deleteStock(std::weak_ptr<StockFactory> pWeakFactory, Stock * pStock)
    {
        std::shared_ptr<StockFactory> pFactory = pWeakFactory.lock();
        if (pFactory != NULL)
        {
            pFactory->removeStock(pStock);
        }
        else
        {
            printf("StockFactory died.\n");
        }
        delete pStock;
    }
    void removeStock(Stock * pStock)
    {
        assert(pStock != NULL);
        m_mapStocks.erase(pStock->getName());
    }

private:
    std::map<std::string, std::weak_ptr<Stock>> m_mapStocks;
};

int main()
{
    // {
    //     std::shared_ptr<StockFactory> pFactory = std::make_shared<StockFactory>();
    //     std::shared_ptr<Stock> pStock = pFactory->get("feng");
    // }

    {
        std::shared_ptr<Stock> pStock;
        std::shared_ptr<StockFactory> pFactory = std::make_shared<StockFactory>();
        pStock = pFactory->get("feng");
    }

    puts("=== ok. ===");

    return 0;
}
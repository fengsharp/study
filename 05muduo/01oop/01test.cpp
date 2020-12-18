#include <stdio.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

class Stock
{
public:
    Stock(const std::string & name)
        : m_strName(name)
    {
        printf("stock %p %s\n", this, m_strName.data());
    }

    ~Stock()
    {
        printf("~stock %p %s\n", this, m_strName.data());
    }

    void print()
    {
        printf("print: stock %p %s\n", this, m_strName.data());
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
    ~StockFactory()
    {
        printf("\tStockFactory delete\n");
    }
    std::shared_ptr<Stock> getStock(const std::string & name)
    {
        std::weak_ptr<Stock> & pWeakStock = m_mapStocks[name];
        std::shared_ptr<Stock> pStock = pWeakStock.lock();
        if (pStock == NULL)
        {
            pStock.reset(new Stock(name), std::bind(StockFactory::weakDeleteStock, std::weak_ptr(shared_from_this()), std::placeholders::_1));
            pWeakStock = pStock;
        }

        return pStock;
    }

private:
    static void weakDeleteStock(std::weak_ptr<StockFactory> pWeakFactory, Stock * pStock)
    {
        std::shared_ptr<StockFactory> pFactory = pWeakFactory.lock();
        if (pFactory != NULL)
        {
            pFactory->removeStock(pStock);
        }
        else
        {
            printf("\tbye!\n");
        }
        delete pStock;
    }

    void removeStock(Stock * pStock)
    {
        if (pStock)
        {
            printf("\tdelete from map, stock %p %s\n", pStock, pStock->getName().data());
            m_mapStocks.erase(pStock->getName());
        }
    }

private:
    std::map<std::string, std::weak_ptr<Stock>> m_mapStocks;
};

int main()
{
    {
        std::shared_ptr<Stock> pStock;
        {
            std::shared_ptr<StockFactory> pFactory = std::make_shared<StockFactory>();
            {
                pStock = pFactory->getStock("feng");
                pStock->print();
            }
        }
    }

    puts("=== over. ===");

    return 0;
}
/*
stock 0x18680e0 feng
print: stock 0x18680e0 feng
        StockFactory delete
        bye!
~stock 0x18680e0 feng
=== over. ===
*/
#include <stdio.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

class Observable;

class Observer : public std::enable_shared_from_this<Observer>
{
public:
    virtual ~Observer();
    virtual void update() = 0;

    void observer(Observable * pSub);

protected:
    Observable * m_pSubject;
};

class Observable
{
public:
    void registerObserver(std::weak_ptr<Observer> pObserver)
    {
        printf("-- %ld\n", pObserver.use_count());
        m_vecObservers.push_back(pObserver);
        printf("-- %ld\n", pObserver.use_count());
    }

    void notify()
    {
        auto it = m_vecObservers.begin();
        while (it != m_vecObservers.end())
        {
            std::shared_ptr<Observer> pObj = it->lock();
            if (pObj)
            {
                pObj->update();
                ++it;
            }
            else
            {
                printf("notify erase observer\n");
                it = m_vecObservers.erase(it);
            }
        }
    }

private:
    std::vector<std::weak_ptr<Observer>> m_vecObservers;
};

Observer::~Observer()
{
}

void Observer::observer(Observable * pSub)
{
    pSub->registerObserver(shared_from_this());
    m_pSubject = pSub;
}

class Foo : public Observer
{
public:
    virtual void update() override
    {
        printf("Foo::update() %p\n", this);
    }
};

int main()
{
    Observable subject;

    {
        std::shared_ptr<Foo> p = std::make_shared<Foo>();
        printf("%p %ld\n", p.get(), p.use_count());
        p->observer(&subject);
        printf("%p %ld\n", p.get(), p.use_count());
        subject.notify();
    }

    subject.notify();

    return 0;
}
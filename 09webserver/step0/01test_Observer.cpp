#include <stdio.h>

#include <algorithm>
#include <iostream>
#include <vector>

class Observable;

class Observer
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
    void registerObserver(Observer * pObserver)
    {
        m_vecObservers.push_back(pObserver);
    }

    void unregisteObserverr(Observer * pObserver)
    {
        if (auto it = std::find(m_vecObservers.begin(), m_vecObservers.end(), pObserver);
            it != m_vecObservers.end())
        {
            std::swap(*it, m_vecObservers.back());
            m_vecObservers.pop_back();
        }
    }

    void notify()
    {
        for (auto & item : m_vecObservers)
        {
            if (item)
            {
                item->update();
            }
        }
    }

private:
    std::vector<Observer *> m_vecObservers;
};

Observer::~Observer()
{
    m_pSubject->unregisteObserverr(this);
}

void Observer::observer(Observable * pSub)
{
    pSub->registerObserver(this);
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
    Foo * p = new Foo;
    Observable subject;
    p->observer(&subject);
    subject.notify();
    delete p;
    subject.notify();

    return 0;
}
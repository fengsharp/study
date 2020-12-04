#include <stdio.h>

#include <list>

class Foo
{
public:
    Foo(int index)
        : m_index(index)
    {
        printf("Foo() %p %d\n", this, m_index);
    }

    ~Foo()
    {
        printf("~Foo() %p %d\n", this, m_index);
    }

    Foo(const Foo & rhs)
        : m_index(rhs.m_index)
    {
        printf("Foo(const Foo & rhs) %p %d\n", this, m_index);
    }

    Foo(Foo && rhs)
        : m_index(rhs.m_index)
    {
        rhs.m_index = 0;
        printf("Foo(const Foo && rhs) %p %d\n", this, m_index);
    }

    Foo & operator=(const Foo & rhs)
    {
        m_index = rhs.m_index;
        printf("operator=(const Foo & rhs) %p %d\n", this, m_index);
        return *this;
    }

    Foo & operator=(Foo && rhs)
    {
        m_index = rhs.m_index;
        rhs.m_index = 0;
        printf("operator=(const Foo && rhs) %p %d\n", this, m_index);
        return *this;
    }
private:
    int m_index;
};

template <typename T>
class TestContainer
{
public:
    void put(T & x)
    {
        m_list.emplace_back(std::move(x));
    }

    T take()
    {
        T front(std::move(m_list.front()));
        m_list.pop_front();
        return front;
    }

private:
    std::list<T> m_list;
};

int main()
{
    Foo f1(100);
    TestContainer<Foo> container;
    container.put(f1);

    printf("-- take\n");
    Foo t = container.take();

    return 0;
}
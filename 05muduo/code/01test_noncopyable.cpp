#include <stdio.h>

#include <utility>
#include <vector>

#include "Noncopyable.h"

class Foo : private NonCopyable
{
};

void test_noncopyalbe()
{
    Foo obj1;
    // Foo obj2(obj1); //  error: call to implicitly-deleted copy constructor of 'Foo'
    Foo obj2;
    // obj2 = obj1; // object of type 'Foo' cannot be assigned because its copy assignment operator is implicitly deleted
    // Foo obj3 = std::move(obj1); // call to implicitly-deleted copy constructor of 'Foo'

    (void)obj1;
    (void)obj2;
}

class TestMove
{
public:
    TestMove()
        : m_val(0)
    {
        printf("TestMove() [%p]\n", this);
    }

    TestMove(const TestMove & rhs)
        : m_val(rhs.m_val)
    {
        printf("TestMove(const TestMove & rhs) [%p] rhs[%p]\n", this, &rhs);
    }

    TestMove(TestMove && rhs)
        : m_val(rhs.m_val)
    {
        rhs.m_val = 0;
        printf("TestMove(TestMove && rhs) [%p] rhs[%p]\n", this, &rhs);
    }

    TestMove & operator=(const TestMove & rhs)
    {
        printf("operator=(const TestMove & rhs) [%p] rhs[%p]\n", this, &rhs);
        return *this;
    }

    TestMove & operator=(const TestMove && rhs)
    {
        printf("operator=(const TestMove && rhs) [%p] rhs[%p]\n", this, &rhs);
        return *this;
    }

private:
    int m_val;
};

void test_move1()
{
    TestMove obj1;
    TestMove && obj2 = std::move(obj1);
    TestMove obj3 = std::move(obj1); // TestMove(TestMove && rhs)
    TestMove obj4;
    obj4 = obj3; // operator=(const TestMove & rhs)
    TestMove obj5;
    obj5 = std::move(obj3); // operator=(const TestMove && rhs)

    printf("obj1[%p] obj2[%p] obj3[%p] obj4[%p] obj5[%p]\n", &obj1, &obj2, &obj3, &obj4, &obj5);
}

void test_move2()
{
    TestMove obj1; 
    TestMove obj2; 
    TestMove obj3; 
    std::vector<TestMove> vecData;
    vecData.reserve(10);

    // TestMove(const TestMove & rhs)
    vecData.push_back(obj1); 

    // TestMove(TestMove && rhs)
    vecData.push_back(std::move(obj2));

    // TestMove(const TestMove & rhs)
    vecData.emplace_back(obj3);

    for (const TestMove& item : vecData)
    {
        printf("[%p] ", &item);
    }
    printf("\n");
}

void test_move3()
{
    std::vector<TestMove> vecData;
    vecData.reserve(10);

    /*
    TestMove() [0x7ffe792fb298]
    TestMove(TestMove && rhs) [0x6c9010] rhs[0x7ffe792fb298]
    */
    vecData.push_back(TestMove()); 

    /*
    TestMove() [0x7ffe792fb290]
    TestMove(TestMove && rhs) [0x6c9014] rhs[0x7ffe792fb290]
    */
    vecData.emplace_back(TestMove());
}

int main()
{
    // test_move1();
    // test_move2();
    test_move3();

    return 0;
}

/*
结论：
1. std::vector emplace_back和push_back 一样，如果传入是右值，则尝试调用移动构造，如果是左值，则拷贝构造
2. std::move 只是将data转为了右值引用，而当右值引用赋值给变量时，才会可能找移动构造
3. 继承了NonCopyable后，对象传参时，必须引用传递，避免拷贝
4. 类的深复制使用移动构造和赋值是有意义的，避免了内存的再次拷贝，但是作为普通的值类型，移动构造并不能带来好处
*/
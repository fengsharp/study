# 1. 学习路径
1. C++多线程系统编程
- 线程安全的对象生命周期管理
- 线程同步
- 多线程常用编程模型
- C++多线程系统编程精要
2. muduo网络库
- base
- net

# 2. C++多线程生命周期管理
## 2.1. 多线程下对象销毁面临的问题
1. 当进入析构函数的时候，如何确定没有其他线程进入了该对象的其他函数
2. 如何保证一个线程在访问对象函数时，其他线程不会销毁该对象
3. 在调用某个对象的成员函数之前，如何得知这个对象还或者？该对象的析构函数会不会碰巧执行到一半

## 2.2 对象创建线程安全
唯一要求是在构造方法中不暴露this指针
1. 不要在构造函数中注册任何回调
2. 不要在构造函数中把this指针传给跨线程的对象
3. 即便在构造函数中的最后一行也不行 因为可能有子类，父类的构造方法结束了，子类的构造方法还没有处理完

```c++
class Foo;
class Observable
{
public:
    void registerObj(Foo * pEle)
    {
        m_vecFoos.push_back(pEle);
    }

    void notify()
    {
        for (auto & item : m_vecFoos)
        {
            item->update();
        }
    }
private:
    std::vector<Foo*> m_vecFoos;
};

class Foo
{
public:
    Foo(Observable * s)
    {
        s->registerObj(this);
        m_pData = new char[1024];
        m_pData.init();
    }

    void update()
    {
        // ...
    }
private:
    void * m_pData;
};

```
Foo(Observable * s) 中将this泄露给了 Observable 对象  
加入threadA上面new了Foo对象，进入了构造方法，注册后线程进入wait  
此时threadB访问s的notify对象，遍历每一个Foo对象，进入Foo的update方法，而此时Foo对象的成员m_pData还没有初始化  

可以通过两段式方式
```c++

class Foo
{
public:
    Foo()
    {
        m_pData = new char[1024];
        m_pData.init();
    }

    void init(Observable * s)
    {
        s->registerObj(this);
    }
    // ...
};

```
## 2.3 对象间的关系
1. 组合关系 Composition

```c++
class Person
{
private:
    std::unique_ptr<Heart> m_pHeart;
};

class Car
{
private:
    std::vector<std::unique_ptr<Wheel>> m_vecWheels;
}
```

2. 聚合关系 Aggregation
```c++
// Custormer 生命周期由外部控制，Shop相当于弱引用，对于Shop而言，不知Custormer合适销毁
class Shop
{
public:
    void enter(Custormer * pCustormer);
    void exit(Custormer * pCustormer);
private:
    std::vector<Custormer*> m_vecCustormers;
}
```

3. 关联关系 Association  
关联关系最为复杂，可以单向关联，双向关联，自身关联， 通过第三方关联，不关联 关联方的生命周期
```c++
class MathTeacher;
class Student
{
private:
    MathTeacher * m_pMathTeacher;
};

class MathTeacher
{
private:
    std::vector<Student*> m_vecStudents;
};

```

4. 依赖关系 Dependency  
```c++
// 类A依赖于B，不作为对象A的数据成员，而作为方法的参数或返回值，或者局部变量
class Student
{
public:
    Computer program();
    void study(Computer * pComputer);
    void playGame()
    {
        Computer computer;
        computer.startGame();
    }
};

```


## 2.4 销毁太难-生命周期  
对于单线程，当使用对象指针时，只需要关注该对象指针是否为空，或者是否是野指针，或者是否是空悬指针（指针指向的地址已经销毁）即可。  
对于单线程能做到谁new，谁delete就基本没有问题了  
```c++

class Foo
{
public:
    ~Foo()
    {
        MutexLockGuard(m_mutex);
        // A ...
    }

    void update()
    {
        MutexLockGuard(m_mutex); // B
        // ...
    }
private:
    MutexLock m_mutex;
};

Foo * pFoo = new Foo();

void fun1()
{
    // threadA 
    delete pFoo;
    pFoo = NULL;
}

void fun2()
{
    if (pFoo != NULL)
    {
        pFoo->update();
    }
}
```

对于单线程，fun1 fun2 无论以何种顺序调用，pFoo对象（去掉锁机制的代码）指针使用都没有问题  
如果是多线程 threadA 进入了注释A处，threadB 进入了注释B处阻塞，而析构函数执行完毕后，锁也给释放了，这时候threadB再去拿锁，行为未知  

从对象与对象之间的关系来看：  
1. 如果是组合关系，A对象管理了B对象的生命周期，则无需在意B对象的析构。B对象的方法调用也无需考虑B对象是否已经销毁
2. 如果是聚合关系或者关联关系，A对象不知道B对象的生命周期，则B对象的方法调用就要额外注意是否进入了B的析构方法
3. 如果是依赖关系，如果是局部变量或者方法返回参数，则线程间基本不会出现共享问题，所以可以视为单线程处理，而作为参数的依赖要看外部传入的多线程问题
```c++
class Observer;

class Observable
{
public:
    void registerObj(Observer * pObj);
    void unregister(Observer * pObj);
    void notifyAll()
    {
        for (auto & item : m_vecObjs)
        {
            item->update(); // B
        }
    }
private:
    std::vector<Observer*> m_vecObjs;
}

class Observer
{
public:
    void registerSelf(Observable * s)
    {
        s->registerObj(this);
        m_pSubject = s;
    }

    void update()
    {
        // ...
    }

    ~Observer()
    {
        m_pSubject->unregister(this);
        // ...
    }

private:
    Observable * m_pSubject;
}
```
Observer对象的析构函数调用unregister，对于多线程而言，首先不能确定m_pSubject对象是否存活。  
鉴于Observer设计模式的特点，假定m_pSubject存在，Observer析构方法里面，不能确定成员方法会不会被调用。  
比如，threadA 进入了析构中，还没有调用到m_pSubject->unregister(this);  
而threadB进入了notifyAll()访问到了Observer对象中的update，此时Observer对象，  
一个线程在析构，一个线程在调用成员方法  

## 2.5 shared_ptr
1. 可能延长生命周期，比如std::bind
2. 传参数时，尽量const T &
3. 当引用为0，析构时，该线程会析构维持的对象

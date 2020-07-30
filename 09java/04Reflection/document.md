# 目的
运行期，对某一个实例一无所知的情况下，如何调用方法

# Class类
1. 除了基本数据类型，其他都是class，包括枚举，数组，字符串，接口
2. jvm负责加载class文件，内存中对应一个Class实例，Class大概
```java
class Class {
    private Class() {}

    // name
    // package
    // constructor methords
    // methords
    // fields
    // extends
    // implements
}
```
3. 获取Class实例
- Person.class
- obj.getClass
- Class.forName(package+name)
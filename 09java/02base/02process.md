# 基础概念
1. jre 包括jvm和核心类库
2. jdk 包括jre和Java tools

# 基本数据类型
- 数值
    - 整形
        - byte(1)
        - short(2)
        - int(4)
        - long(8)
    - 浮点
        - float(4)
        - double(8)
- 字符 char(2) 2字节刚好是Unicode内存占用字节
- 布尔 boolean 没有定义字节大小，jvm内部表示为4字节
- 整形如何变为unsigned类型
```java
Byte 
    static int toUnsignedInt(byte x)  
    static long toUnsignedLong(byte x)  

Short
    static int toUnsignedInt(short x)  
    static long toUnsignedLong(short x)  

Integer
    static long toUnsignedLong(int x)  


```

# 字符
1. char 内存中表示一个Unicode字符
2. 存储或者网络传输时，是utf-8格式
3. 可以使用字面量赋值，也可以用编码赋值
```java
char a = '中';
char b = '\u4e2d';
```

# 数组
1. 数组长度一旦确定不可变
2. 数组初始化有默认值
3. 索引超出范围报错 java.lang.ArrayIndexOutOfBoundsException
4. 声明方式
```java
int[] arr1 = new int[10];
int[] arr2 = new int[] {1, 2, 3};
int[] arr3 = {1, 2, 3};
```

# demo
1. 遍历数组，计算和 fori
2. 遍历数组，计算和 foreach
3. 打印数组内容，Arrays
4. 排序
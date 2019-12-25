## type
1. base
    1. num
        1. integer
            - byte
            - short
            - int
            - long
            - long long
        2. float
            - float
            - double
    2. string
        char
2. define construt
    - arr
    - struct
    - union
    - enum
3. pointer
    - base data*
    - struct*
    - function

## 常量和变量
内存分区
- os
- environment parameters
- stack
- share
- heap
- data
    - bss
    - data initialized data segment
    - read only data
- code

1. 字符串常量会放在 read only data
2. 静态的变量放在 initialized data segment
3. 全局未初始化的变量 放在 bss
4. 普通变量放在函数栈帧

## 声明和定义
声明 extern int a;  
定义 int a;  
初始化 int a = 3; or int a; a = 3;  

## 进制
b 位  
B 字节  
1B = 8b  

## c语言进制表示
十进制  
八进制  0123
16进制  0x123
二进制  不能直接书写二进制  

## 打印进制
%d %o %x  
%f %lf
%d %hd
%u %ul %ull
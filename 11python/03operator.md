# 1. 基础知识
## 1. 数值运算符
```
+ - * / % 
```

## 2. 比较运算符

```
> < >= <=  ==
```


# 2. 代码示例
## 1. 简单
1. 数值型计算
2. 字符串与数值型计算
```python

# 数值型计算
print(1+2)
print(10/3)
print(10%3)

# 字符串与数值只有*可以使用
print("5" * 3)
print("hello" * 3)

```
结果：
```
3
3.3333333333333335
1
555
hellohellohello
```

## 2. 提高
熟悉python获取键盘输入接口input，和print对应，print是告诉显卡硬件打印到屏幕，而input是告诉键盘硬件，把用户输入的内容读出来，然后一般要放入的内存中，当然用一个变量接收

Python3.x 中 input() 函数接受一个标准输入数据，返回为 string 类型。所以下面的代码不能达到计算数值运算的目的
```python
# 熟悉input
# input([prompt]) prompt: 提示信息 可以不写
a = input("输入第一个数值：")
b = input("输入第二个数值：")
# print打印由很多格式，不必过于追究细节
print("计算结果:", a+b)
```
结果是：
```
输入第一个数值：1
输入第二个数值：3
计算结果: 13
```

但从数据角度看，经常碰到的是数值类型和字符串类型，不可避免的会碰到字符串变为数值进行计算，数值编程字符串进行拼接打印的需求
1. 字符串变为数值
``` python
i = int("123")
f = float("23.33")
```

2. 数值变为字符串
```python
# result: 123456
s = str(123) + str(456)
```

所以上面的代码进行修改
```python
# 熟悉input
# input([prompt]) prompt: 提示信息 可以不写
strA = input("输入第一个数值：")
strB = input("输入第二个数值：")
# 可以 a = int(input("输入第一个数值："))
a = int(strA)
b = int(strB)
# print打印由很多格式，不必过于追究细节
print("计算结果:", a+b)
```

结果是：
```
4
```

3. string to bytes
```python
msg = "hello, world"
print("msg: ", msg)
msgBytes = str.encode(msg, "utf-8")
print("msgBytes: ", msgBytes)
```
结果
```
msg:  hello, world
msgBytes:  b'hello, world'
```

4. bytes to string
```python
msgBytes = b"hello, world"
print("msgBytes: ", msgBytes)
msg = bytes.decode(msgBytes, 'utf-8')
print("msg: ", msg)
```
结果
```
msgBytes:  b'hello, world'
msg:  hello, world
```
# 1. 语法概念
function和后续的class都是强有力的抽象工具，人有了抽象才会在一个高层次上抛开过于细节问题来综合考虑问题。  
比如：写技术标书，在通读了招标文件后，写文档怎么写？首先得先来个大纲，这就是站在高层上面看问题，比如有概述 软件需求 软件功能 技术难点 售后服务 付款方式等大纲，然后接下来是对每一个大纲进行细化，写软件需求，就把注意力局限在需要哪些软件，比如arcgis，此时就不需要考虑软件功能的大纲里面的内容，否则就思维混乱了  
同样的对于代码，复杂的逻辑拆开来考虑，再逐个进行分析解决，始终让大脑处于较小复杂问题的思路范围内，不要发散，不要纠结，逐个问题解决后，就解决了所有的问题，应和了重过程追结果的做事哲学：完成一件事情如果需要100个环节，那每个环节都不能少  

## 概念
1. 调用  
之前使用的系统函数 print input int() float() 就是系统调用
2. 自定义
```
    def [function name] ([param1, parm2,]):
        ...
```
    需要关注：
    1. 名字-给function起个好的名字
    2. 考虑输入有哪些-参数定义
    3. 考虑返回结果是什么，可以返回多个参数
3. 递归
    可以暂时不考虑，很少情况会碰到这样的情况

# 2. 实践
1. 实现function 计算平方
```python
def power(x):
    return x * x

print("3 * 3 = ", power(3))
```
结果：
```
3 * 3 =  9
```

# 3. 小结
学习到这里，需要复习所有内容，基本概念应该都能理解，重点语法应该能够熟练应用，接下来需要练习代码进行巩固，能熟练解决问题后，进入类和常用库学习阶段  
另外该阶段可以查阅资料熟悉字符串处理接口，以及了解列表的使用  
1. 输入用户名，打印 hello, []
2. 判断玩家输入的用户名和密码是否正确，提高：登录界面和登录逻辑分别封装为function
3. 猜数字，使用系统调用random，通过搜索 python random，查看如何产生随机数
4. 随机生成数学成绩，打印数学成绩，并判断成绩等级
5. 和计算机进行石头剪头布游戏， 提高：三局两胜
6. 打印九九乘法表
7. 扩展，查阅资料，了解字符串 列表 字典常用接口
    1. 字符串长度
    2. 字符串索引访问
    3. 字符串切片获取子串
    4. 字符串拼接
    5. 字符串与数值间的相互转换
```python
sentence = 'tom\'s pet is a cat'  # 单引号中间还有单引号，可以转义
	sentence2 = "tom's pet is a cat"  # 也可以用双引号包含单引号
	sentence3 = "tom said:\"hello world!\""
	sentence4 = 'tom said:"hello world"'
	# 三个连续的单引号或双引号，可以保存输入格式，允许输入多行字符串
	words = """
	hello
	world
	abcd"""
	print(words)
	
	py_str = 'python'
	len(py_str)  # 取长度
	py_str[0]  # 第一个字符
	'python'[0]
	py_str[-1]  # 最后一个字符
	# py_str[6]  # 错误，下标超出范围
	py_str[2:4]  # 切片，起始下标包含，结束下标不包含
	py_str[2:]  # 从下标为2的字符取到结尾
	py_str[:2]  # 从开头取到下标是2之前的字符
	py_str[:]  # 取全部
	py_str[::2]  # 步长值为2，默认是1
	py_str[1::2]  # 取出yhn
	py_str[::-1]  # 步长为负，表示自右向左取
	
	py_str + ' is good'  # 简单的拼接到一起
	py_str * 3  # 把字符串重复3遍
	
	't' in py_str  # True
	'th' in py_str  # True
	'to' in py_str  # False
	'to' not in py_str  # True
```
    6. 列表长度
    7. 列表索引获取
    8. 列表切片
    9. 列表添加内容
    10. 列表删除内容
```python
**alist = [10, 20, 30, 'bob', 'alice', [1,2,3]]
	len(alist)
	alist[-1]  # 取出最后一项
	alist[-1][-1]  # 因为最后一项是列表，列表还可以继续取下标
	[1,2,3][-1]  # [1,2,3]是列表，[-1]表示列表最后一项
	alist[-2][2]  # 列表倒数第2项是字符串，再取出字符下标为2的字符
	alist[3:5]   # ['bob', 'alice']
	10 in alist  # True
	'o' in alist  # False
	100 not in alist # True
	alist[-1] = 100  # 修改最后一项的值
	alist.append(200)  # 向**列表中追加一项
```
    11. tuple与列表的异同
    12. tuple长度 切片 索引获取
```python
atuple = (10, 20, 30, 'bob', 'alice', [1,2,3])
	len(atuple)
	10 in atuple
	atuple[2]
	atuple[3:5]
	# atuple[-1] = 100  # 错误，元组是不可变的
```
    13. 字典初始化 长度 判断键是否存在 通过键获取值
```python
# 字典是key-value(键－值）对形式的，没有顺序，通过键取出值
	adict = {'name': 'bob', 'age': 23}
	len(adict)
	'bob' in adict  # False
	'name' in adict  # True
	adict['email'] = 'bob@tedu.cn'  # 字典中没有key，则添加新项目
	adict['age'] = 25  # 字典中已有key，修改对应的value
```
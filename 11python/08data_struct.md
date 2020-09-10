# 08. 基本数据结构
## 08.01 分类
- string
- list
- tunnel
- map
## 08.02 string
1. 索引
```python
str = "abcdef"
# str[0] is "a"
# str[3] is "d"
# 下标索引从0开始
```

2. 切片  
目的：取子串  
切片的语法：[起始:结束:步长]  
注意：选取的区间属于左闭右开型，即从"起始"位开始，到"结束"位的前一位结束(不包含结束位本身)  

```python
name = 'abcdef'
print(name[0:3])  # result: "abc"
print(name[0:5])  # result: "abcde"
print(name[3:5])  # result: "de"
print(name[2:])   # result: "cdef"
print(name[2:-1]) # result: "cde"
# note: 从右向左，索引从-1开始，所以-1只的是最后一个索引，而切片的结束位置不能包含，所以取到右侧后就结束
print(name[::2])  # result: "ace"  
```

3. find  
检测 str 是否包含在 mystr中，如果是返回开始的索引值，否则返回-1  
mystr.find(str, start=0, end=len(mystr))  
rfind 了解 类似于 find()函数，不过是从右边开始查找.  
```python
mystr = 'hello world'
index = mystr.find("world") # result: 6
index = mystr.find("none")  # result: -1

```

4. count 了解  
返回 str在start和end之间 在 mystr里面出现的次数  
mystr.count(str, start=0, end=len(mystr))  
```python
mystr = 'hello world'
cnt = mystr.count("world") # result 1
```

5. replace  
把 mystr 中的 str1 替换成 str2,如果 count 指定，则替换不超过 count 次.  
mystr.replace(str1, str2,  mystr.count(str1))  
```python
mystr = 'hello world'
result = mystr.replace("hello", "world") # result: "world world"
# note：此时mystr没有变，仍然是'hello world'，替换后的结果在result
```

6. splite 特别常见  
以 str 为分隔符切片 mystr  
```python
mystr = 'hello world'
result = mystr.splite(" ") # 以空格分割，返回的是list
print(result) # result: ['hello', 'world']
```

7. capitalize 了解  
把字符串的第一个字符大写  
```python
mystr = 'hello world'
result = mystr.capitalize()
print(result) # result: 'Hello world'
```

8. title 了解  
把字符串的每个单词首字母大写  
```python
mystr = 'hello world'
result = mystr.title()
print(result) # result: 'Hello World'
```

9. startswith 常用  
检查字符串是否是以 obj 开头, 是则返回 True，否则返回 False  
mystr.startswith(obj)  
```python
mystr = 'hello world'
print(mystr.startswith('hello')) # result: True
```

10. endswith 了解  
检查字符串是否以obj结束，如果是返回True,否则返回 False.  
mystr.endswith(obj)  
```python
mystr = 'hello world'
print(mystr.endswith('world')) # result: True
```

11. lower 了解  
转换 mystr 中所有大写字符为小写  
mystr.lower() 
```python
mystr = 'Hello World'
result = mystr.lower()
print(result) # result: 'hello world'
```

12. upper 了解   
转换 mystr 中的小写字母为大写

13. 删除字符串空白字符 常用  
lstrip 删除 mystr 左边的空白字符  
rstrip 删除 mystr 字符串末尾的空白字符  
strip  删除mystr字符串两端的空白字符  
```python
mystr = ' Hello World '
result = mystr.strip() # result: 'Hello World'
```

14. isalpha  
如果 mystr 所有字符都是字母 则返回 True,否则返回 False
```python
mystr = 'Hello'
print(mystr.isalpha()) # True
mystr = "123"
print(mystr.isalpha()) # False
```

15. isdigit  
如果 mystr 只包含数字则返回 True 否则返回 False.
```python
mystr = 'Hello'
print(mystr.isalpha()) # False
mystr = "123"
print(mystr.isalpha()) # True
```

16. isalnum  
如果 mystr 所有字符都是字母或数字则返回 True,否则返回 False
```python
mystr = 'Hello World'
print(mystr.isalpha()) # False 因为有空格
mystr = 'Hello'
print(mystr.isalpha()) # True
mystr = "123"
print(mystr.isalpha()) # True
```

## 08.03 list  
思维模型：数组、集合、一组数据，内存中连续存放的数据  
1. 定义：
```python
arr = [1, 2, 3, 4, 5]
arr = [1, "string", True, 3.1415926]
# arr里面的数值类型可以不同，应用时，尽量数据类型一致，原因在于往往需要loop循环处理数据，如果数据类型不同，很难一致处理
```

2. 获取数据  
arr[0] 索引从0开始，len-1结束

3. 循环遍历方式
- 方式1：for
```python
arr = [1, 2, 3, 4, 5]
for item in arr:
    data = item * 10
    print(data)
```
- 方式2：while 需要借住len方法，获取长度
```python
arr = [1, 2, 3, 4, 5]
arrlen = len(arr)
index = 0
while index < arrlen: # 注意是< 而不是<=
    data = item * 10
    print(data)
    index += 1        # 时刻注意while condition状态修改，否则死循环
```

4. 增加元素 
- 方式1：append 必会  
```python

arr = ["bao1", "bao2"]
arr.append("mother")
print(arr) # ['bao1', 'bao2', 'mother']
```

- 方式2：extend 主要用于一个list内元素追加到另一个list中，区别与append
```python
arr1 = [1, 2, 3]
arr2 = [4, 5]
arr1.extend(arr2)
print(arr1)       # result: [1, 2, 3, 4, 5]
```
append 是将list作为一个数据类型加入到list中
```python
arr1 = [1, 2, 3]
arr2 = [4, 5]
arr1.append(arr2)
print(arr1)       # result: [1, 2, 3, [4, 5]]
```

方式3：insert方式
```python
arr = [1, 2, 3]
arr.insert(1, 100)
print(arr)        # result: [1, 100, 2, 3]
```

5. 修改数据
```python
arr = ["dabao1", "dabao2"]
print(arr)        # ['dabao1', 'dabao2']
arr[0] = "mother"
arr[1] = "father"
print(arr)        # ['dabao1', 'dabao2']
```

6. 查找元素
- 方式1：in
```python
arr = ["dabao1", "dabao2", "mother"]
print("father" in arr) # False
print("mother" in arr) # Ture
```
对应的有 not in
- 方式2：index
```python
arr = ["dabao1", "dabao2", "mother"]
# print(arr.index("father")) # 因为不存在，会报错退出
print(arr.index("mother")) # 2
```

- 方式3：count
```python
arr = ["dabao1", "dabao2", "mother"]
print(arr.count("father")) # 0
print(arr.count("mother")) # 1
```

7. 删除元素
- 方式1 pop 删除最后一个元素
```python
arr = ["dabao1", "dabao2", "mother"]
delElement = arr.pop()
print(arr)         # ['dabao1', 'dabao2']
print(delElement)  # mother
```

- 方式2 del 按照索引删除
```python 
arr = ["dabao1", "dabao2", "mother"]
del arr[2]
print(arr)         # ['dabao1', 'dabao2']
```

- 方式3 remove 根据元素的值进行删除
```python
arr = ["dabao1", "dabao2", "mother", "dabao1"]
arr.remove("dabao1")
print(arr)         # ['dabao2', 'mother', 'dabao1']
arr.remove("dabao1")
print(arr)         # ['dabao1', 'dabao2']
```
常用方式1和方式2

8. reverse 逆序
```python
arr = ["dabao1", "dabao2", "mother"]
arr.reverse()
print(arr)         # ['mother', 'dabao2', 'dabao1']
```

9. sort 排序
```python
arr = [3, 5, 2, 7]
print(arr)  # [3, 5, 2, 7]
arr.sort()
print(arr)  # [2, 3, 5, 7]
```

## 08.04 tuple 元组  
Python的元组与列表类似，不同之处在于元组的元素不能修改。元组使用小括号，列表使用方括号。  
arr = (1, 2, 3)
1. 访问
```python
arr = (3, 5, 2, 7)
print(arr)  # (3, 5, 2, 7)
print(arr[2]) # 2
```

2. 不允许修改元组里面现有的值
3. 当然也不能删除，增加元素
4. count index两个接口和list一样

## 08.05 map 字典  
既能存储多个数据，还能在访问元素的很方便就能够定位到需要的那个元素  
语法：以{}表示  
1. 定义
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
print(info) # {'id': 100, 'name': 'xiaobao', 'age': 1}
```

2. 按照键访问
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
print(info['name']) # xiaobao
```
若访问的键不存在，报错

3. get方法访问
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
print(info.get('name')) # xiaobao
```
如果键不存在，也不报错

4. 修改元素
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
info['name'] = 'dabao'
info['age'] = 4
print(info) # {'id': 100, 'name': 'dabao', 'age': 4}
```

5. 添加元素
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
info['sex'] = '男'
print(info) # {'id': 100, 'name': 'xiaobao', 'age': 1, 'sex': '男'}
```

6. 删除元素
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
del info['age']
print(info) # {'id': 100, 'name': 'xiaobao'}
info.clear() # 删除所有内容
print(info) # {}
```

7. 数据个数，长度
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
print(len(info))     # 3
```

8. 返回所有的keys
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
print(info.keys()) # ['id', 'name', 'age']
```

9. 返回所有的值 values
10. items 不常用
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
print(info.items()) # [('id', 100), ('name', 'xiaobao'), ('age', 1)]
```

11. 判断key是否存在 类似list，in | not in判断
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
print('id' in info) # True
print('sex' in info) # False
```

## 08.06 遍历数据
遍历太常见，单独列出来
- 字符串遍历
```python
str = "hello"
for charItem in str:
    print(charItem, end='-') # h-e-l-l-o-
```

- list遍历
```python
arr = [1, 2, 3, 4, 5]
for item in arr:
    print(item, end='-') # 1-2-3-4-5-
```

- 元组遍历  
和list一致

- map遍历
因其结构复杂，所以遍历相对于上面稍显复杂，结构问什么复杂呢？  
对比list，list的数据都是一致的属性，相当于一行存放  
而map每一个数据项，都是有键-值对组成的  
可以将map的结构理解为两列来看待  
info = {'id':100, 'name':'xiaobao', 'age': 1}  
相当于  

| 'id'   | 100       |
| ------ | --------- |
| 'name' | 'xiaobao' |
| 'age'  | 1         |

list相当于一列数据，map相当于2列数据，一行数据是一个数据项
1. 遍历key
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
for keyItem in info.keys():
    print(keyItem, end='-') # id-name-age-
```

2. 遍历value
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
for valueItem in info.values():
    print(valueItem, end='-') # 100-xiaobao-1-
```

3. 遍历数据项-常用
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
for item in info.items():
    print(item)
# ('id', 100)
# ('name', 'xiaobao')
# ('age', 1)
# item是元组，所以item[0]是key item[1]是值
```
当然更常见的是每次拿到数据项，直接访问键和值的方式
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
for keyItem, valueItem in info.items():
    print(keyItem, '-', valueItem)

# id - 100
# name - xiaobao
# age - 1
```
map的遍历，最后一种最常用

4. for遍历的时候，往往有需要知道当前拿到的是第几个数据
- 方式1
```python
index = 0
info = {'id':100, 'name':'xiaobao', 'age': 1}
for keyItem, valueItem in info.items():
    print(index, ':', keyItem, '-', valueItem)
    index += 1

# 0 : id - 100
# 1 : name - xiaobao
# 2 : age - 1
```

- 方式2 使用enumerate
```python
info = {'id':100, 'name':'xiaobao', 'age': 1}
for index, (keyItem, valueItem) in enumerate(info.items()):
    print(index, ':', keyItem, '-', valueItem)

# 0 : id - 100
# 1 : name - xiaobao
# 2 : age - 1
```

## 08.07 内置方法
1. max min
```python
arr = [1, 2, 3, 7, 5, 4]
print(max(arr)) # 7
print(min(arr)) # 1
```

2. len


## 08.08 检核
统计字符串中，各个字符的个数  
比如："hello world" 字符串统计的结果为： h:1 e:1 l:3 o:2 d:1 r:1 w:1

```python
# main start
str = 'hello world'
statistics = {}  # key: cnt
for charItem in str:
    if not charItem.isalpha(): # 排除空格 数字
        continue
    if charItem in statistics:
        statistics[charItem] += 1
    else:
        statistics[charItem] = 1

for i, (key, value) in enumerate(statistics.items()):
    print(i, ':', key, ' ', value)
    
# 0 : h   1
# 1 : e   1
# 2 : l   3
# 3 : o   2
# 4 : w   1
# 5 : r   1
# 6 : d   1
```

在获取map数据的时候，可以充分利用get方法特性简化代码  
get:第一个参数是key，如果找打该key，就返回对应的值，如果没有找到，就返回get参数的第二个默认值
```python
# main start
str = 'hello world'
statistics = {}  # key: cnt
for charItem in str:
    if not charItem.isalpha(): # 排除空格 数字
        continue
    cnt = statistics.get(charItem, 0)
    statistics[charItem] = cnt + 1

for i, (key, value) in enumerate(statistics.items()):
    print(i, ':', key, ' ', value)
    
# 0 : h   1
# 1 : e   1
# 2 : l   3
# 3 : o   2
# 4 : w   1
# 5 : r   1
# 6 : d   1
```


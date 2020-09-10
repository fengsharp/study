# 1. 语法
有了循环，+条件就可以干很多事情了  
```python

for item in [集合]:
    # item表示每次从集合中拿到一个 依次取完后结束

while condition:
    # ...

# 满足condition就执行下面代码，代码执行完毕
# 再次回到while处，判断condition条件
# 成立继续，不成立就结束

# break 是在循环内部终端外侧的循环
# continue 是在循环内部打断后续代码，进行下次循环
# 具体语法在实践中体会记忆

# 需要补充的是，有必要在此处了解集合的语法，类似数组
# array = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
# python还提供了简单的方式
# array = range(10) 和上面的一样，从0开始，[0, 10)
```
# 2. 实践
1. 使用 for array方式计算0+1+2+3+ ... + 9
2. 使用 for range(10)方式计算
3. 使用while 计算
```python
# 1. for array
arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
arrSum = 0
for item in arr:
    arrSum = arrSum + item
print("arrSum:", arrSum)

# 2. for range
rangeSum = 0
for item in range(10):
    rangeSum += item
print("rangeSum:", rangeSum)

# 3. while
whileSum = 0
index = 0
while index < 10:
    whileSum += index
    index += 1
print("whileSum:", whileSum)

```

结果：
```
arrSum: 45
rangeSum: 45
whileSum: 45
```

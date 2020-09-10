# 1. 语法
条件和循环是计算机自动化执行任务的重要条件。没有条件判断和循环执行，就没有了一切。  
```python
# 一个分支
if condition:
    ...

# 两个分支
if condition:
    ...
else:
    ...

if condition:
    ...
elif condition: # else if 简写
    ...
else:
    ...
```
# 2. 实践
问题：输入数学成绩，>=90 优秀, >= 75 中等, >= 60 及格，低于60 不及格

```python
# condition
strScore = input("输入数学成绩：")
score = int(strScore)

if score >= 90:
    print("优秀")
elif score >= 75:
    print("中等")
elif score >= 60:
    print("及格")
else:
    print("不及格")
```
结果：
```
输入数学成绩：85
```
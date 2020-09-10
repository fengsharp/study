1. 输入用户名，打印 hello, []
```python
name = input("input your name: ")
print("hello ", name)
```
result:
```
input your name: feng
hello  feng
```

2. 判断玩家输入的用户名和密码是否正确，提高：登录界面和登录逻辑分别封装为function
```python
userName = input("input user name: ")
password = input("input password: ")

if userName == "feng" and password == "love":
    print("login suc")
else:
    print("login fail")
```

result:

```
input user name: feng
input password: l
login fail
-------------------------------
input user name: feng
input password: love
login suc
```

登录界面和登录逻辑分别封装为function

```python
def loginUi():
    userName = input("input user name: ")
    password = input("input password: ")
    return userName, password

def loginLogic(userName, password):
    if userName == "feng" and password == "love":
        return True
    else:
        return False

# main start
userName, pwd = loginUi()
if loginLogic(userName, pwd):
    print("login suc")
else:
    print("login fail")
```
    体会抽象：
    1. main start中抽象获取用户输入，然后逻辑判断，输出结果
    2. 用户输入fuction内部再考虑如何获取用户输入的用户名和密码
    3. 逻辑判断function内部再考虑如何判断输入信息正确

3.  猜数字，使用系统调用random，通过搜索 python random，查看如何产生随机数
```python
# 导入额外的库
import random

def generateNum(minNum, maxNum):
    return random.randrange(minNum, maxNum)

# main
# 常量(不变量)，定义名字最好大写，提醒不要修改值
FIX_MAX_NUM = 10
FIX_MIN_NUM = 1
print("num range:[%d, %d]"%(FIX_MIN_NUM, FIX_MAX_NUM))

num = generateNum(FIX_MIN_NUM, FIX_MAX_NUM)
# 成功退出
while True:
    userGuess = int(input("- input guess num: "))
    if userGuess == num:
        print("right!")
        break # exit loop
    elif userGuess > num:
        print("your guess num > num")
    else:
        print("your guess num < num")
```
result:
```
num range:[1, 10]
- input guess num: 5
your guess num < num
- input guess num: 8
your guess num > num
- input guess num: 7
right!
```

4. 随机生成数学成绩，打印数学成绩，并判断成绩等级
```python
import random

def queryScore():
    return random.randrange(0, 100)

# main start
math = queryScore()
if math >= 90:
    print(math, "A")
elif math >= 75:
    print(math, "B")
elif math >= 60:
    print(math, "C")
else:
    print(math, "D")
```
result:
```
65 C
```

5. 和计算机进行石头剪头布游戏， 提高：三局两胜
```python

import random

ENUM_SCISSORS = 1
ENUM_STONE = 2
ENUM_CLOTH = 3

def inputUi():
    machineType = random.randrange(ENUM_SCISSORS, ENUM_CLOTH)
    userType = int(input("input your choise(1-scissors 2-stone 3-cloth): "))
    return machineType, userType

def printRaceResult(machineType, userType):
    typeDir = {ENUM_SCISSORS: "scissors", ENUM_STONE: "stone", ENUM_CLOTH: "cloth"}
    print("machine choise: %s, user choise: %s"%(typeDir[machineType], typeDir[userType]))

# main start
raceCnt = 3 # race count
machineWinCnt = 0
userWinCnt = 0
while raceCnt > 0 and abs(machineWinCnt - userWinCnt) < 2:
    machineType, userType = inputUi()
    printRaceResult(machineType, userType)

    if machineType == userType:
        print(" same, again!")
        continue
    elif abs(machineType - userType) == 1:
        machineWinCnt += 1 if machineType > userType else 0
        userWinCnt += 0 if machineType > userType else 1
    elif abs(machineType - userType) == 2:
        machineWinCnt += 0 if machineType > userType else 1
        userWinCnt += 1 if machineType > userType else 0
    else:
        print("err case")
    
    raceCnt -= 1

if machineWinCnt > userWinCnt:
    print("machine win! %d:%d"%(machineWinCnt, userWinCnt))
else:
    print("user win!%d:%d"%(machineWinCnt, userWinCnt))

```

6. 打印九九乘法表
```python
for row in range(1, 9+1):
    for col in range(1, row+1):
        print("%d * %d = %d"%(col, row, col * row), end=" ") # end 确保不换行
    print() # 换行
```


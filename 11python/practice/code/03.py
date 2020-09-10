
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

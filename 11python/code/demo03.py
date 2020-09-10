
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
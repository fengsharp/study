
for row in range(1, 9+1):
    for col in range(1, row+1):
        print("%d * %d = %d"%(col, row, col * row), end=" ") # end 确保不换行
    print() # 换行
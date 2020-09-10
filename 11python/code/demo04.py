
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
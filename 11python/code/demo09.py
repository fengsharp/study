

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

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
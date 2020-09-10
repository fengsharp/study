# 1. 语法
- 类
- 方法
- 成员
- 内部封装 前缀 __

```python
class Student(object):
    def __init__(self, name, score):
        self.__name = name
        self.__score = score

    def getName(self):
        return self.__name
    
    def getScore(self):
        return self.__score

    def getGrade(self):
        if self.__score >= 90:
            return "A"
        elif self.__score >= 60:
            return "B"
        else:
            return "C"
    
lisa = Student("lisa", 22)
print("%s grade: %s"%(lisa.getName(), lisa.getGrade()))
```

- __name是私有变量，外部不能访问，如果是[name], 则外部可以访问
- 如果方法想私有，同样的 def __functionName()
- 命名规则1：class类名首字母大写，驼峰方式
- 命名规则2：function首字母小写，驼峰方式
- 命名规则3：变量首字母小写，驼峰方式
- 命名规则4：常量全部大写，单词间用下划线分割
- class名称尽量为名词，如果是动作，尽可能归类为一种命令类内
- 方法尽可能是动词
- 继承方式可以查阅资料了解即可，能不用继承就不用继承的方式，代码复用，继承会增加复杂度，但不是说要禁用继承，该用的时候还得用

# 2. 实践
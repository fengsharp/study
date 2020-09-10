
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
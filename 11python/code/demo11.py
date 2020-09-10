


class Person(object):
    def __init__(self, name='', phone=''):
        self.__name = name
        self.__phone = phone

    def getName(self):
        return self.__name
    
    def getPhone(self):
        return self.__phone
    
    def setName(self, name):
        self.__name = name

    def setPhone(self, phone):
        self.__phone = phone

    def print(self):
        print(self.__name, '\t', self.__phone)

class DbConnection(object):
    def __init__(self):
        self.__fileName = "person.txt"

    def findAllPerson(self):
        try:
            lstPerson = []
            file = open(self.__fileName, "r")
            while True:
                lineContent = file.readline()
                if not lineContent:
                    break
                else:
                    splits = lineContent.split(' ')
                    p = Person(splits[0], splits[1])
                    lstPerson.append(p)
            file.close()
            return lstPerson
        except :
            return []

    def findPerson(self, name):
        try:
            file = open(self.__fileName, "r")
            while True:
                lineContent = file.readline()
                if not lineContent:
                    break
                else:
                    splits = lineContent.split(' ')
                    if name == splits[0]:
                        p = Person(splits[0], splits[1])
                        return p
                    else:
                        continue
            file.close()
            return None
        except:
            return None

    def add(self, person):
        file = open(self.__fileName, "a")
        file.write(person.getName() + " " + person.getPhone()+"\n")
        file.close()

    def save(self, persons):
        file = open(self.__fileName, "w")
        for p in persons:
            file.write(p.getName() + " " + p.getPhone()+"\n")
        file.close()

class ContactsService(object):
    def __init__(self):
        self.__dbConn = DbConnection()

    def findAll(self):
        return self.__dbConn.findAllPerson()

    def find(self, name):
        return self.__dbConn.findPerson(name)

    def add(self, person):
        return self.__dbConn.add(person)

    def remove(self, name):
        persons = self.__dbConn.findAllPerson()
        for p in persons:
            if p.getName() == name:
                persons.remove(p)
                break
        self.__dbConn.save(persons)


class UiWindow(object):
    def __init__(self):
        self.__service = ContactsService()

    def displayMainUi(self):
        print('''
        0. 退出系统，输入0
        1. 查看所有，输入1
        2. 查看联系人，输入2
        3. 添加联系人，输入3
        4. 删除联系人，输入4
        ''')
        op = int(input("输入选择："))
        return op

    def showAll(self):
        print("--- show all")
        for p in self.__service.findAll():
            p.print()
        print("------------")
        
    def query(self):
        print("--- quer ---")
        name = input("输入要查找的姓名：")
        p = self.__service.find(name)
        if p is not None:
            p.print()
        else:
            print("通讯录中没有该用户")
        print("------------")

    def add(self):
        print("--- add ---")
        name = input("输入联系人姓名：")
        p = self.__service.find(name)
        if p is not None:
            print("该姓名已经存在，不提供更新功能，可删除再添加")
            return
        else:
            phone = input("请输入手机号：")
            self.__service.add(Person(name, phone))
            print("添加完成")
        print("-----------")
    
    def remove(self):
        print("--- remove ---")
        name = input("输入联系人姓名：")
        p = self.__service.find(name)
        if p is None:
            print("通讯录中没有该用户！")
            return
        else:
            self.__service.remove(name)
        print("-----------")
    

# main start
winObj = UiWindow()
while True:
    op = winObj.displayMainUi()
    if op == 0:
        break
    elif op == 1:
        # show all 
        winObj.showAll()
    elif op == 2:
        # show one
        winObj.query()
    elif op == 3:
        # add
        winObj.add()
    elif op == 4:
        # del
        winObj.remove()
    else:
        print("不存在该操作")
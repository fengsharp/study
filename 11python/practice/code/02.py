
# userName = input("input user name: ")
# password = input("input password: ")

# if userName == "feng" and password == "love":
#     print("login suc")
# else:
#     print("login fail")


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
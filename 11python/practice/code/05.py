
import random

ENUM_SCISSORS = 1
ENUM_STONE = 2
ENUM_CLOTH = 3

def inputUi():
    machineType = random.randrange(ENUM_SCISSORS, ENUM_CLOTH)
    userType = int(input("input your choise(1-scissors 2-stone 3-cloth): "))
    return machineType, userType

def printRaceResult(machineType, userType):
    typeDir = {ENUM_SCISSORS: "scissors", ENUM_STONE: "stone", ENUM_CLOTH: "cloth"}
    print("machine choise: %s, user choise: %s"%(typeDir[machineType], typeDir[userType]))

# main start
raceCnt = 3 # race count
machineWinCnt = 0
userWinCnt = 0
while raceCnt > 0 and abs(machineWinCnt - userWinCnt) < 2:
    machineType, userType = inputUi()
    printRaceResult(machineType, userType)

    if machineType == userType:
        print(" same, again!")
        continue
    elif abs(machineType - userType) == 1:
        machineWinCnt += 1 if machineType > userType else 0
        userWinCnt += 0 if machineType > userType else 1
    elif abs(machineType - userType) == 2:
        machineWinCnt += 0 if machineType > userType else 1
        userWinCnt += 1 if machineType > userType else 0
    else:
        print("err case")
    
    raceCnt -= 1

if machineWinCnt > userWinCnt:
    print("machine win! %d:%d"%(machineWinCnt, userWinCnt))
else:
    print("user win!%d:%d"%(machineWinCnt, userWinCnt))


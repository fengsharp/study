
srcFilePath = "del.txt"
fileFlag = srcFilePath.rfind(".")
fileFormat = srcFilePath[fileFlag:] # .txt
fileName = srcFilePath[:fileFlag] # del
targetFilePath = fileName + "_copy" + fileFormat
print(targetFilePath)

srcFile = open(srcFilePath, "r")
targetFile = open(targetFilePath, "w")

for line in srcFile.readlines():
    targetFile.write(line)

targetFile.close()
srcFile.close()
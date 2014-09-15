'''
使用subprocess.Popen调用adb命令将/system/lib下的所有库pull出来
'''
import shlex
import subprocess
import os

cmdls = "adb shell ls /system/lib/libsk_ca*"
outPath = "libs"

'''
创建目录
'''
if not os.access(outPath, os.R_OK):
    os.mkdir(outPath, mode=0o777)

'''
分割cmd
'''
args = shlex.split(cmdls)
p = subprocess.Popen(args, stdout=subprocess.PIPE)
fileNames = p.stdout.readlines()
for fileName in fileNames:
    fileStr = str(fileName, encoding='utf-8')
    fileStr = fileStr[:fileStr.rfind(".so")+3]
    print(fileStr)
    cmdpull = "adb pull " + fileStr + " " + outPath
    args = shlex.split(cmdpull)
    subprocess.Popen(args)

print("func end")


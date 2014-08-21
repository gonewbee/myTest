'''
调用shell命令，将当前目录下的so库push到/system/lib下
'''
import os

files = os.listdir()
for fp in files:
    if fp.endswith("*.so"):
        os.system("adb push "+fp+" /system/lib")

os.system("adb shell sync")

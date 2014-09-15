import os
os.system("adb remount")

names = ["dvbserver", "com.taixin.dtvapp", "com.share.dcas"]
#names = ["dvbserver", "com.taixin.dtvapp"]

for name in names:
    str = "adb shell ps | grep " + name
    ret = os.popen(str)
    s = ret.read()
    splits = s.split()
    for pid in splits[1:-1]:
        if pid!='':
            break
    print(name+" "+pid)
    str = "adb shell kill -s 9 " + pid
    os.system(str)
    

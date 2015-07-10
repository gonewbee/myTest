#coding=utf-8
'''
调用so库，使用X11显示图像
'''
import os
import time
from ctypes import *

libc = cdll.LoadLibrary("./draw_continue.so")

#按时间遍历文件夹中的文件
path = 'tmp'
mtime = lambda f: os.stat(os.path.join(path, f)).st_mtime
files = list(sorted(os.listdir(path), key=mtime))

winId = "3a0017e"#"3a000fc"
libc.init_display()
for f in files:
    print(f)
    if f.find(winId)>=0:
        f = os.path.join(path, f)
        f_s = c_char_p(f)
        print(f_s)
        libc.draw_window(f_s)
        time.sleep(0.5)
time.sleep(20)
libc.destroy_display()




import os
import time
from ctypes import *

libc = cdll.LoadLibrary("./draw.so")
#libc.draw_tmp_file(b"tmp/433_216_500_335_433_216_500_335_0_0_2000_1436161197_843139")

path = 'tmp'
mtime = lambda f: os.stat(os.path.join(path, f)).st_mtime
files = list(sorted(os.listdir(path), key=mtime))

for f in files:
    f = os.path.join(path, f)
    f_s = c_char_p(f)
    print(f_s)
    libc.draw_tmp_file(f_s)
    #time.sleep(1)



# coding=utf-8
import numpy as np
import cv2
from os import listdir
from os.path import isfile, join
import re

mypath = "tmp"
files = [f for f in listdir(mypath) if isfile(join(mypath, f))]

for f in files:
    print(f)
    with open(mypath + "/" + f, "rb") as fi:
        img = np.zeros((1024, 1024, 4), np.uint8)
        w, h = map(int, re.search('(\d+)x(\d+)', f).groups())
        s = fi.read()
        t = bytearray(s)
        print("%x%x%x %x " % (t[0], t[1], t[2], t[3]))
        for x in range(w):
            for y in range(h):
                p = (x * h + y) * 4
                img[y, x] = [t[p + 2], t[p], t[p + 1], t[p + 3]]
        cv2.imshow('image', img)
        cv2.waitKey(0)
        cv2.destroyAllWindows()

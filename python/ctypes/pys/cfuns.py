import os
from ctypes import CDLL, CFUNCTYPE, c_char_p, c_int

clib_path = os.path.dirname(os.path.abspath(__file__))
clib_path = os.path.dirname(clib_path)
clib_path = os.path.join(clib_path, 'cfuns', 'libcfuns.so')
cfuns = CDLL(clib_path)


def add(a, b):
    return cfuns.add(a, b)


def getLen(s):
    return cfuns.getLen(s)


def hello(s):
    return cfuns.hello(s)


def rigister_str_func():
    CALLBACK = CFUNCTYPE(c_int, c_char_p)
    cfuns.registerStrCallback(CALLBACK(py_str_func))


def py_str_func(s):
    print("%s len:%d" % (s, len(s)))
    return len(s)

if __name__ == '__main__':
    rigister_str_func()

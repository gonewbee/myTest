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


def run_callback():
    cfuns.runCallback()


def cstrargs(*args):
    t = cfuns.cstrargs(len(args), *args)
    print(t)


def arrtest():
    array_type = c_char_p * 4
    names = array_type()

    names[0] = 'test1'
    names[1] = 'test2'
    names[2] = 'test3'
    names[3] = 'test4'

    cfuns.arrtest(4, names)
    for t in names:
        print(t)


def py_str_func(s):
    print(type(s))
    return len(s)

if __name__ == '__main__':
    # rigister_str_func()
    # run_callback()
    cstrargs('abc', '123', 'defghi', 'jihg')
    arrtest()

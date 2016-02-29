"""
python中函数传递变量时传递的都是引用，只是有些是可变对象有些是不可变对象
"""


def argInt(a):
    """
    整数是不可变对象类型，当发送值发送变化后通过id函数发现地址也变化了
    """
    print("In argInt a id:%d value:%d" % (id(a), a))
    a += 10
    print("In argInt a id:%d value:%d" % (id(a), a))


def argList(l):
    """
    list是可变对象类型
    """
    print("In argList l id:%d value:" % (id(l)), end=' ')
    print(l)
    l.append(10)
    print("In argList l id:%d value:" % (id(l)), end=' ')
    print(l)

if __name__ == '__main__':
    i = 10
    print("In main before call func i id:%d value:%d" % (id(i), i))
    argInt(i)
    print("In main after call func i id:%d value:%d" % (id(i), i))
    l = [1, 2, 3]
    print("In main before call func  l id:%d value:" % (id(l)), end=' ')
    print(l)
    argList(l)
    print("In main after call func  l id:%d value:" % (id(l)), end=' ')
    print(l)

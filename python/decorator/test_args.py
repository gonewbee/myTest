"""使用修饰符decorator判断参数类型"""


def check_arg(func):
    def wrapper(l, *args):
        if not isinstance(l, list):
            raise Exception("l must be list")
        print(func.__name__ + " was called!")
        return func(l, *args)
    return wrapper


@check_arg
def test_func(l, n):
    t = [i + n for i in l]
    print(t)

if __name__ == '__main__':
    n = 10
    l = list(range(10))
    test_func(l, n)
    l = 10
    test_func(l, n)

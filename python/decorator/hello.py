def runtimes(rtimes=2):
    print('rtimes:' + str(rtimes))
    def fn_times(fn):
        print('in fn_times:' + str(fn))
        def do_fn_times():
            for i in range(rtimes):
                fn()
        return do_fn_times
    print('in runtimes')
    return fn_times

@runtimes(3)
def foo():
    print("i am foo")

def bar():
    print("i am bar")

if __name__ == '__main__':
    print('enter main')
    foo()
    print("========")
    runtimes()(bar)()

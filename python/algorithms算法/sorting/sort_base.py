class SortBase:
    """docstring for SortBase"""
    
    @staticmethod
    def sort(a):
        raise NotImplementedError( "Should have implemented this" )
        
    @staticmethod
    def less(v, w):
        # python3中没有cmp函数了
        return v < w

    @staticmethod
    def exch(a, i, j):
        """交换"""
        t = a[i]
        a[i] = a[j]
        a[j] = t

    @staticmethod
    def show(a):
        for x in a:
            print(x, end=" ")
        print()

    @staticmethod
    def isSorted(a):
        for i in range(1, len(a)):
            if SortBase.less(a[i], a[i-1]):
                return False
        return True

    @classmethod
    def test(cls, filename):
        """
        使用classmethod可以利用传入的cls调用对应的staticmethod
        测试排序函数，可用测试algs4-data下文件：
        tiny.txt words3.txt
        """
        with open(filename, 'r') as fi:
            a = []
            for l in fi.readlines():
                a += l.split()
            cls.sort(a)
            # assert断言
            assert cls.isSorted(a)
            cls.show(a)



import sys
from sort_base import SortBase

class ShellSort(SortBase):
    """希尔排序Shellsort"""

    @staticmethod
    def sort(a):
        print("Use ShellSort")
        cls = ShellSort
        n = len(a)
        h = 1
        while h < n//3:
            h = h*3 + 1
        while h >= 1:
            for i in range(h, n):
                j = i
                while j >= h and cls.less(a[j], a[j-h]):
                    cls.exch(a, j, j-h)
                    j -= h
            h = h//3

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Need input file')
        sys.exit(1)
    ShellSort.test(sys.argv[1])

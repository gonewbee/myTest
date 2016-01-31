import sys
from sort_base import SortBase

class InsertionSort(SortBase):

    @staticmethod
    def sort(a):
        """插入排序法"""
        print("Use InsertionSort")
        n = len(a)
        cls = InsertionSort
        for i in range(1, n):
            j = i
            while j > 0 and cls.less(a[j], a[j-1]):
                cls.exch(a, j, j-1)
                j -= 1

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Need input file')
        sys.exit(1)
    InsertionSort.test(sys.argv[1])

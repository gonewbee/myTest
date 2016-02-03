import sys
from sort_base import SortBase

class QuickSort(SortBase):
    """快速排序"""
    
    @staticmethod
    def sort(a):
        print("Use QuickSort")
        QuickSort.sort_partition(a, 0, len(a)-1)

    @classmethod
    def sort_partition(cls, a, lo, hi):
        if hi <= lo:
            return
        j = cls.partition(a, lo, hi)
        cls.sort_partition(a, lo, j-1)
        cls.sort_partition(a, j+1, hi)
        
    @classmethod
    def partition(cls, a, lo, hi):
        v = a[lo]
        i = lo+1
        j = hi
        while True:
            while cls.less(a[i], v):
                if i == hi:
                    break
                i += 1
            while cls.less(v, a[j]):
                if j == lo:
                    break
                j -= 1
            if i >= j:
                break
            cls.exch(a, i, j)
        cls.exch(a, lo, j)
        return j
                    
if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Need input file')
        sys.exit(1)
    QuickSort.test(sys.argv[1])   
        

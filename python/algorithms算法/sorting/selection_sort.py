import sys
from sort_base import SortBase

class SelectionSort(SortBase):
    """选择排序"""
    
    @staticmethod
    def sort(a):
        """每次循环遍历选择最小的"""
        print("Use SelectionSort")
        n = len(a)
        for i in range(n):
            min = i
            for j in range(i+1, n):
                if SelectionSort.less(a[j], a[min]):
                    min = j
            SelectionSort.exch(a, i, min)
        
if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Need input file')
        sys.exit(1)
    SelectionSort.test(sys.argv[1])

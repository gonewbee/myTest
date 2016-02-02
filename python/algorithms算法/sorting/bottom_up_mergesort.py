import sys
from merge_sort import MergeSort

class BottomUP(MergeSort):
    
    @staticmethod
    def sort(a):
        print("Use BottomUP MergeSort")
        n = len(a)
        BottomUP.aux = [0]*n
        sz = 1
        while sz < n:
            lo = 0
            while lo < n-sz:
                BottomUP.merge(a, lo, lo+sz-1, min(lo+sz+sz-1, n-1))
                lo += (sz+sz)
            sz = sz + sz
            
if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Need input file')
        sys.exit(1)
    BottomUP.test(sys.argv[1])
        

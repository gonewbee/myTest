import sys
from merge_sort import MergeSort

class TopDown(MergeSort):

    @staticmethod
    def sort(a):
        print("Use TopDown MergeSort")
        TopDown.aux = [0]*len(a)
        TopDown.sort_merge(a, 0, len(a)-1)

    @staticmethod
    def sort_merge(a, lo, hi):
        if hi <= lo:
            return
        mid = lo + (hi-lo)//2
        TopDown.sort_merge(a, lo, mid)
        TopDown.sort_merge(a, mid+1, hi)
        TopDown.merge(a, lo, mid, hi)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Need input file')
        sys.exit(1)
    TopDown.test(sys.argv[1])

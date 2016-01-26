import sys
import binarySearch

def count(d):
    d = sorted(d)
    n = len(d)
    cnt = 0
    for i in range(n):
        for j in range(i, n):
            if (binarySearch.rank(-d[i]-d[j], d) > j):
                cnt += 1
    return cnt
    
if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('need input file name')
        sys.exit(1)
    with open(sys.argv[1], 'r') as fi:
        d = [int(x) for x in fi.readlines()]
        cnt = count(d)
        print(cnt)
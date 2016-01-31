# 二分查找
import sys
import fileinput
import time

def rank(key, a):
    # if not isinstance(a, list):
    #     print('rank input error!')
    #     return -1
    lo = 0
    hi = len(a) - 1
    while lo <= hi:
        mid = lo + (hi-lo)//2
        if key < a[mid]:
            hi = mid - 1
        elif key > a[mid]:
            lo = mid + 1
        else:
            return mid
    return -1

def binarySearch(fileW, fileT):
    with open(fileW, 'r') as wf:
        # 按行读入并转换为int类型
        whitelist = [int(x) for x in wf.readlines()]
        # 将list排序，二分查找需要数组是有序的
        whitelist = sorted(whitelist)
        with open(fileT, 'r') as tf:
            for line in [int(x) for x in tf.readlines()]:
                if rank(line, whitelist) < 0:
                    print(line)
    
if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('need input white file and test file')
        sys.exit(1)
    start_time = time.time()
    binarySearch(sys.argv[1], sys.argv[2])
    print("--- %s seconds ---" % (time.time() - start_time))

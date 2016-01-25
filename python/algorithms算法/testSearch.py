import sys
import fileinput
import time

def rank(key, a):
    # if not isinstance(a, list):
    #     print('rank input error!')
    #     return -1
    length = len(a)
    i = 0
    while i < length:
        if a[i] == key:
            return i
        i += 1
    return -1

def testSearch(fileW, fileT):
    with open(fileW, 'r') as wf:
        # 按行读入并转换为int类型
        whitelist = [int(x) for x in wf.readlines()]
        with open(fileT, 'r') as tf:
            for line in [int(x) for x in tf.readlines()]:
                if rank(line, whitelist) < 0:
                    print(line)

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('need input white file and test file')
        sys.exit(1)
    start_time = time.time()
    testSearch(sys.argv[1], sys.argv[2])
    print("--- %s seconds ---" % (time.time() - start_time))
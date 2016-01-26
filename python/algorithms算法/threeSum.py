# 统计数组中三个数和为0的数量
import sys

def three_count(d):
    if not isinstance(d, list):
        print('three_count need list')
        return -1
    n = len(d)
    cnt = 0
    for i in range(n):
        for j in range(i, n):
            for k in range(j, n):
                if (d[i]+d[j]+d[k]) == 0:
                    cnt += 1
    # i = 0
    # while i < n:
    #     j = i + 1
    #     while j < n:
    #         k = j + 1
    #         while k < n:
    #             if (d[i]+d[j]+d[k]) == 0:
    #                 cnt += 1
    #             k += 1
    #         j += 1
    #     i += 1
    return cnt
    
if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('need input file')
        sys.exit(1)
    with open(sys.argv[1], 'r') as fi:
        d = [int(x) for x in fi.readlines()]
        cnt = three_count(d)
        print(cnt)  
                  

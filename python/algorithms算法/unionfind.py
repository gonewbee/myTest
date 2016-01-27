# Union-Find(并查集)的简单实现

import sys

class UF:
    def __init__(self, N):
        # 集合数量
        self.count = N
        self.id = list(range(N))
    
    def counts(self):
        return self.count
        
    def connected(self, p, q):
        """判断是否相连"""
        return self.find(p) == self.find(q)
        
    def find(self, p):
        """获取所属集合"""
        return self.id[p]
        
    def union(self, p, q):
        """连接两个集合"""
        pID = self.find(p)
        qID = self.find(q)
        if pID == qID:
            return
        for i in range(len(self.id)):
            if self.id[i] == pID:
                self.id[i] = qID
        self.count -= 1
        
if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Nedd input file!!')
        sys.exit(1)
    with open(sys.argv[1], 'r') as fi:
        N = int(fi.readline())
        uf = UF(N)
        for l in fi.readlines():
            p, q = [int(x) for x in l.split()]
            if uf.connected(p, q):
                continue
            uf.union(p, q)
            print('%d %d' % (p, q))
        print('%d components' % (uf.counts()))

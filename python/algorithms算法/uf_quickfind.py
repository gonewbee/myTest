# Union-Find(并查集)快速查找的实现

import sys
from unionfind import UF

class QuickFind(UF):
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
        uf = QuickFind(N)
        for l in fi.readlines():
            p, q = [int(x) for x in l.split()]
            if uf.connected(p, q):
                continue
            uf.union(p, q)
            print('%d %d' % (p, q))
        print('%d components' % (uf.counts()))

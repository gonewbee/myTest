# Union-Find(并查集)快速合并的实现

import sys
from unionfind import UF

class QuickUnion(UF):

    def find(self, p):
        """获取所属集合"""
        # 找出集合名称
        while p != self.id[p]:
            p = self.id[p]
        return p
        
    def union(self, p, q):
        """连接两个集合"""
        # 将集合合并
        pRoot = self.find(p)
        qRoot = self.find(q)
        if pRoot == qRoot:
            return
        self.id[pRoot] = qRoot
        self.count -= 1
        
if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Nedd input file!!')
        sys.exit(1)
    with open(sys.argv[1], 'r') as fi:
        N = int(fi.readline())
        uf = QuickUnion(N)
        for l in fi.readlines():
            p, q = [int(x) for x in l.split()]
            if uf.connected(p, q):
                continue
            uf.union(p, q)
            print('%d %d' % (p, q))
        print('%d components' % (uf.counts()))

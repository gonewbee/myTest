import sys
from unionfind import UF

class WeightedQuickUnion(UF):
    """加权quick-union算法"""
    def __init__(self, N):
        super(WeightedQuickUnion, self).__init__(N)
        self.sz = [1 for x in self.id]

    def find(self, p):
        while p != self.id[p]:
            p = self.id[p]
        return p

    def union(self, p, q):
        i = self.find(p)
        j = self.find(q)
        if i == j:
            return
        if self.sz[i] < self.sz[j]:
            self.id[i] = j
            self.sz[j] += self.sz[i]
        else:
            self.id[j] = i
            self.sz[i] += self.sz[j]
        self.count -= 1

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Nedd input file!!')
        sys.exit(1)
    with open(sys.argv[1], 'r') as fi:
        N = int(fi.readline())
        uf = WeightedQuickUnion(N)
        for l in fi.readlines():
            p, q = [int(x) for x in l.split()]
            if uf.connected(p, q):
                continue
            uf.union(p, q)
            print('%d %d' % (p, q))
        print('%d components' % (uf.counts()))


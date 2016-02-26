from depth_first_order import check_G, DepthFirstOrder


class KosarajuSCC():
    """kosaraju算法计算有向图的强联通分量"""

    @check_G
    def __init__(self, G):
        self.counts = 0
        self.markeds = [False]*G.V()
        self.ids = [0]*G.V()
        order = DepthFirstOrder(G.reverse())
        for s in order.reversePost():
            if not self.markeds[s]:
                self.dfs(G, s)
                self.counts += 1

    @check_G
    def dfs(self, G, v):
        self.markeds[v] = True
        self.ids[v] = self.counts
        for w in G.adj(v):
            if not self.markeds[w]:
                self.dfs(G, w)

    def stronglyConnected(self, v, w):
        return self.ids[v] == self.ids[w]

    def id(self, v):
        return self.ids[v]

    def count(self):
        return self.counts

if __name__ == '__main__':
    """
    $ python connected_components.py ~/Data/learn/algs4-data/tinyG.txt
    3 components
    6 5 4 3 2 1 0
    8 7
    12 11 10 9
    """
    from digraph import Digraph
    import sys
    if len(sys.argv) < 2:
        print('Need input file')
        sys.exit(1)
    with open(sys.argv[1], 'r') as f:
        g = Digraph(f)
        cc = KosarajuSCC(g)
        m = cc.count()
        print('%d components' % (m))
        components = [0] * m
        for i in range(m):
            components[i] = []
        for v in range(g.V()):
            components[cc.id(v)].insert(0, v)
        for i in range(m):
            for v in components[i]:
                print(v, end=' ')
            print()

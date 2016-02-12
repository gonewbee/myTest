class CC():
    """使用深度优先搜索找出图中所有的连通分量"""

    def __init__(self, g):
        self.marked = [False] * g.V()
        self.counts = 0
        self.ids = [0] * g.V()
        for s in range(g.V()):
            if not self.marked[s]:
                self.dfs(g, s)
                self.counts += 1

    def dfs(self, g, v):
        self.marked[v] = True
        self.ids[v] = self.counts
        for w in g.adj(v):
            if not self.marked[w]:
                self.dfs(g, w)

    def connected(self, v, w):
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
    from graph import Graph
    import sys
    if len(sys.argv) < 2:
        print('Need input file')
        sys.exit(1)
    with open(sys.argv[1], 'r') as f:
        g = Graph(f)
        cc = CC(g)
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

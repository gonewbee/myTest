from _io import TextIOWrapper


class Graph():
    """使用邻接表数组定义无向图"""

    def __init__(self, d):
        if isinstance(d, int):
            self.vs = d
            self.es = 0
            self.adjs = [0] * d
            for i in range(d):
                self.adjs[i] = []
        elif isinstance(d, TextIOWrapper):
            v = int(d.readline())
            e = int(d.readline())
            self.__init__(v)
            for i in range(e):
                x, y = [int(t) for t in d.readline().split()]
                self.addEdge(x, y)

    def addEdge(self, v, w):
        self.adjs[v].insert(0, w)
        self.adjs[w].insert(0, v)
        self.es += 1

    def adj(self, v):
        return self.adjs[v]

    def V(self):
        return self.vs

    def E(self):
        return self.es

    def __repr__(self):
        s = str.format("%d vertices, %d edges\n" % (self.vs, self.es))
        for v in range(self.vs):
            s += str.format("%d: " % (v))
            for w in self.adj(v):
                s += str.format("%d " % (w))
            s += "\n"
        return s

if __name__ == "__main__":
    """
    测试tinyG.txt
    """
    import sys
    if len(sys.argv) < 2:
        print("Need input file!")
        sys.exit(1)
    with open(sys.argv[1], 'r') as f:
        g = Graph(f)
        print(g)

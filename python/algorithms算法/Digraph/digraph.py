from _io import TextIOWrapper


class Digraph():
    """有向图"""

    def __init__(self, arg):
        if isinstance(arg, int):
            self.vs = arg
            self.es = 0
            self.adjs = [0] * arg
            for i in range(arg):
                self.adjs[i] = []
        elif isinstance(arg, TextIOWrapper):
            v = int(arg.readline())
            e = int(arg.readline())
            self.__init__(v)
            for i in range(e):
                x, y = [int(t) for t in arg.readline().split()]
                self.addEdge(x, y)

    def addEdge(self, v, w):
        self.adjs[v].insert(0, w)
        self.es += 1

    def V(self):
        return self.vs

    def E(self):
        return self.es

    def adj(self, v):
        return self.adjs[v]

    def reverse(self):
        r = Digraph(self.vs)
        for v in range(self.vs):
            for w in self.adjs[v]:
                r.addEdge(w, v)
        return r

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
    测试tinyDG.txt
    """
    import sys
    if len(sys.argv) < 2:
        print("Need input file!")
        sys.exit(1)
    with open(sys.argv[1], 'r') as f:
        g = Digraph(f)
        print(g)

from _io import TextIOWrapper
from edge import Edge


class EdgeWeightedGraph():
    """加权无向图"""

    def __init__(self, I):
        if isinstance(I, int):
            self.v = I
            self.e = 0
            self.adjs = [0] * I
            for i in range(I):
                self.adjs[i] = []
        elif isinstance(I, TextIOWrapper):
            v = int(I.readline())
            e = int(I.readline())
            self.__init__(v)
            for i in range(e):
                x, y, w = I.readline().split()
                edge = Edge(int(x), int(y), float(w))
                self.addEdge(edge)

    def V(self):
        return self.v

    def E(self):
        return self.e

    def addEdge(self, e):
        v = e.either()
        w = e.other(v)
        self.adjs[v].insert(0, e)
        self.adjs[w].insert(0, e)
        self.e += 1

    def adj(self, v):
        return self.adjs[v]

    def edges(self):
        b = []
        for v in range(self.V()):
            for e in self.adj(v):
                if e.other(v) > v:
                    b.append(e)
        return b

if __name__ == '__main__':
    import sys
    if len(sys.argv) < 2:
        print("Need enter file!")
        sys.exit()
    with open(sys.argv[1], 'r') as f:
        g = EdgeWeightedGraph(f)
        print(g.edges())

from graph import Graph


class SymbolGraph:
    """用符号作为顶点名的图，符号图"""

    def __init__(self, filename, sp):
        self.st = {}
        with open(filename, 'r') as f:
            while True:
                line = f.readline()
                if line == '':
                    break
                line = line.split('\n')[0]
                for a in line.split(sp):
                    if a not in self.st:
                        self.st[a] = len(self.st)
            self.keys = [0] * len(self.st)
            for name in self.st.keys():
                self.keys[self.st[name]] = name
        self.g = Graph(len(self.st))
        with open(filename, 'r') as f:
            while True:
                line = f.readline()
                if line == '':
                    break
                line = line.split('\n')[0]
                a = line.split(sp)
                v = self.st[a[0]]
                for t in a[1:]:
                    self.g.addEdge(v, self.st[t])
        # print(self.st)

    def contains(self, s):
        return s in self.st

    def index(self, s):
        return self.st[s]

    def name(self, v):
        return self.keys[v]

    def G(self):
        return self.g

if __name__ == '__main__':
    """
    $ python symbol_graph.py ~/Data/learn/algs4-data/routes.txt " "
    JFK
        ORD
        ATL
        MCO
    LAX
        LAS
        PHX
    $ python symbol_graph.py ~/Data/learn/algs4-data/movies.txt '/'
    """
    import sys
    if len(sys.argv) < 3:
        print('Input error!')
        sys.exit(1)
    sg = SymbolGraph(sys.argv[1], sys.argv[2])
    g = sg.G()
    while True:
        source = input()
        for w in g.adj(sg.index(source)):
            print('\t' + sg.name(w))

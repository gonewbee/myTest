from digraph import Digraph


def check_G(func):
    def wrapper(self, G, *args):
        if not isinstance(G, Digraph):
            raise Exception("G must be Digraph!")
        func(self, G, *args)
    return wrapper


class DirectedCycle():
    """检测有向图中是否含有环"""

    @check_G
    def __init__(self, G):
        self.markeds = [False] * G.V()
        self.edgeTo = [0] * G.V()
        self.cycles = []
        self.onStack = [False] * G.V()
        for v in range(G.V()):
            if not self.markeds[v]:
                self.dfs(G, v)

    @check_G
    def dfs(self, G, v):
        self.onStack[v] = True
        self.markeds[v] = True
        for w in G.adj(v):
            if self.hasCycle():
                return
            elif not self.markeds[w]:
                self.edgeTo[w] = v
                self.dfs(G, w)
            elif self.onStack[w]:
                x = v
                while x != w:
                    self.cycles.insert(0, x)
                    x = self.edgeTo[x]
                self.cycles.insert(0, w)
                self.cycles.insert(0, v)
        self.onStack[v] = False

    def hasCycle(self):
        return len(self.cycles) != 0

    def cycle(self):
        return self.cycles

if __name__ == '__main__':
    import sys
    if len(sys.argv) < 2:
        print("Enter need file!")
        sys.exit(1)
    with open(sys.argv[1], 'r') as f:
        g = Digraph(f)
        diCycle = DirectedCycle(f)
        if diCycle.hasCycle():
            print("Has cycle:", end=' ')
            print(diCycle.cycle())

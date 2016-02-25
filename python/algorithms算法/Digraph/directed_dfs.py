from digraph import Digraph


class DirectedDFS():
    """有向图深度优先搜索，计算可达性"""

    def __init__(self, G, s):
        if not isinstance(G, Digraph):
            print("G must be Digraph!")
            return
        self.markeds = [False]*G.V()
        if isinstance(s, int):
            self.dfs(G, s)
        elif isinstance(s, list):
            for i in s:
                if not self.markeds[i]:
                    self.dfs(G, i)

    def dfs(self, G, v):
        if not isinstance(G, Digraph):
            print("G must be Digraph")
            return
        self.markeds[v] = True
        for w in G.adj(v):
            if not self.markeds[w]:
                self.dfs(G, w)

    def marked(self, v):
        return self.markeds[v]

if __name__ == '__main__':
    import sys
    if len(sys.argv) < 3:
        print("Enter argv less\n")
        sys.exit(1)

    with open(sys.argv[1]) as f:
        g = Digraph(f)
        s = [int(i) for i in sys.argv[2:]]
        # print(s)

        reachable = DirectedDFS(g, s)
        for v in range(g.V()):
            if reachable.marked(v):
                print(v, end=' ')
        print()

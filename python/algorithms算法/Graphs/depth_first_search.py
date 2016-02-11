from search import Search


class DepthFirstSearch(Search):
    """深度优先搜索"""

    def __init__(self, g, s):
        self.counts = 0
        self.markeds = [False]*g.V()
        self.dfs(g, s)

    def dfs(self, g, v):
        self.markeds[v] = True
        self.counts += 1
        for w in g.adj(v):
            if not self.markeds[w]:
                self.dfs(g, w)

    def marked(self, w):
        return self.markeds[w]

    def count(self):
        return self.counts

class DepthFirstPaths:
    """使用深度优先搜索查找图中的路径"""

    def __init__(self, g, s):
        self.marked = [False] * g.V()
        self.edgeTo = [0] * g.V()
        self.s = s
        self.dfs(g, s)

    def dfs(self, g, v):
        self.marked[v] = True
        for w in g.adj(v):
            if not self.marked[w]:
                self.edgeTo[w] = v
                self.dfs(g, w)

    def hasPathTo(self, v):
        return self.marked[v]

    def pathTo(self, v):
        if not self.hasPathTo(v):
            return None
        path = []
        x = v
        while x != self.s:
            path.insert(0, x)
            x = self.edgeTo[x]
        path.insert(0, self.s)
        return path

from paths import Paths


class BreadthFirstPaths(Paths):
    """使用广度优先搜索查找图中的路径"""

    def __init__(self, g, s):
        self.marked = [False] * g.V()
        self.edgeTo = [0] * g.V()
        self.s = s
        self.bfs(g, s)

    def bfs(self, g, s):
        queue = []
        self.marked[s] = True
        queue.append(s)
        while len(queue) != 0:
            v = queue.pop(0)
            for w in g.adj(v):
                if not self.marked[w]:
                    self.marked[w] = True
                    self.edgeTo[w] = v
                    queue.append(w)

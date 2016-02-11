class Paths:
    """查找图中路径类"""

    def __init__(self, g, s):
        raise NotImplementedError("Should have implemented this")

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

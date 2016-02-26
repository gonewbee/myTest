from digraph import Digraph


def check_G(func):
    def wrapper(self, G, *argv):
        if not isinstance(G, Digraph):
            raise Exception("G need be Digraph")
        return func(self, G, *argv)
    return wrapper


class DepthFirstOrder():
    """有向图中基于深度优先搜索的顶点排序"""

    @check_G
    def __init__(self, G):
        self.pres = []
        self.posts = []
        self.reversePosts = []
        self.markeds = [False]*G.V()
        for v in range(G.V()):
            if not self.markeds[v]:
                self.dfs(G, v)

    @check_G
    def dfs(self, G, v):
        self.pres.append(v)
        self.markeds[v] = True
        for w in G.adj(v):
            if not self.markeds[w]:
                self.dfs(G, w)
        self.posts.append(v)
        self.reversePosts.insert(0, v)

    def pre(self):
        return self.pres

    def post(self):
        return self.posts

    def reversePost(self):
        return self.reversePosts


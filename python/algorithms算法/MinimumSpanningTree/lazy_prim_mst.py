from min_pq import MinPQ
from mst import MST


class LazyPrimMST(MST):
    """最小生成树的Prim算法的延时实现"""

    def __init__(self, G):
        self.pq = MinPQ()  # 横切边（包括失效的边）
        self.marked = [False] * G.V()  # 最小生成树的顶点
        self.mst = []  # 最小生成树的边
        self.visit(G, 0)  # 假设G是联通的
        while not self.pq.isEmpty():
            # 遍历横切边
            e = self.pq.delMin()  # 取最小的边
            v = e.either()
            w = e.other(v)
            if self.marked[v] and self.marked[w]:
                # 如果边的两个顶点都已经在树里就跳过这个边
                continue
            self.mst.append(e)  # 将边加入最小生成树
            if not self.marked[v]:
                self.visit(G, v)
            if not self.marked[w]:
                self.visit(G, w)

    def visit(self, G, v):
        """
        将顶点加入最小生成树
        """
        self.marked[v] = True
        for e in G.adj(v):
            # 将另一个顶点不在最小生成树中的邻接边加入横切边中
            if not self.marked[e.other(v)]:
                self.pq.insert(e)

    def edges(self):
        return self.mst

    def weight(self):
        weights = 0
        for e in self.mst:
            weights += e.weight()
        return weights

if __name__ == '__main__':
    """
    $ python lazy_prim_mst.py ~/Data/learn/algs4-data/tinyEWG.txt
    0-7 0.16
    1-7 0.19
    0-2 0.26
    2-3 0.17
    5-7 0.28
    4-5 0.35
    6-2 0.40
    1.81
    """
    import sys
    if len(sys.argv) < 2:
        print("Input need file")
        sys.exit(1)
    LazyPrimMST.test(sys.argv[1])

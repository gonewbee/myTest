class MinPQ(object):
    """docstring for MinPQ"""

    def __init__(self):
        self.pq = []

    def isEmpty(self):
        return len(self.pq) == 0

    def insert(self, e):
        self.pq.append(e)
        # 按照边的权重进行排序
        self.pq.sort(key=lambda x: x.weight())

    def delMin(self):
        return self.pq.pop(0)

class MST():
    """docstring for MST"""

    def __init__(self, G):
        raise NotImplementedError("Should have implemented this")

    def edges(self):
        raise NotImplementedError("Should have implemented this")

    def weight(self):
        raise NotImplementedError("Should have implemented this")

    @classmethod
    def test(cls, fileName):
        from edge_weighted_graph import EdgeWeightedGraph
        with open(fileName, 'r') as f:
            g = EdgeWeightedGraph(f)
            mst = cls(g)
            for e in mst.edges():
                print(e)
            print(mst.weight())

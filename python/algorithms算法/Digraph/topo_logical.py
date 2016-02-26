from depth_first_order import DepthFirstOrder, check_G
from directed_cycle import DirectedCycle
from symbol_graph import SymbolGraph


class TopoLogical(object):
    """有向图中顶点拓扑排序"""

    @check_G
    def __init__(self, G):
        self.orders = []
        cyclefinder = DirectedCycle(G)
        if not cyclefinder.hasCycle():
            dfs = DepthFirstOrder(G)
            self.orders = dfs.reversePost()
        else:
            print("G hasCycle:", end=' ')
            print(cyclefinder.cycle())

    def order(self):
        return self.orders

    def isDAG(self):
        return len(self.orders) == 0

if __name__ == '__main__':
    """
    $ python topo_logical.py ~/Data/learn/algs4-data/jobs.txt  /
    """
    import sys
    if len(sys.argv) < 3:
        print("Input need file!")
        sys.exit(1)
    symbol = SymbolGraph(sys.argv[1], sys.argv[2])
    top = TopoLogical(symbol.G())
    print(top.order())
    for i in top.order():
        print(symbol.name(i))

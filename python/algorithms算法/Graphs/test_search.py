from graph import Graph
from depth_first_search import DepthFirstSearch
import sys

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Need input file!')
        sys.exit(1)
    with open(sys.argv[1], 'r') as f:
        g = Graph(f)
        s = int(sys.argv[2])
        search = DepthFirstSearch(g, s)
        for v in range(g.V()):
            if search.marked(v):
                print(v, end=' ')
        print()
        if search.count != g.V():
            print('Not', end=' ')
        print('Connected')

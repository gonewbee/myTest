from graph import Graph
from depth_first_paths import DepthFirstPaths
import sys

if __name__ == '__main__':
    """
    测试使用tinyCG.txt
    $ python test_paths.py ~/Data/learn/algs4-data/tinyCG.txt 0
    0 to 0: 0
    0 to 1: 0-2-1
    0 to 2: 0-2
    0 to 3: 0-2-3
    0 to 4: 0-2-3-4
    0 to 5: 0-2-3-5
    """
    if len(sys.argv) < 3:
        print('Need input file!')
        sys.exit(1)
    with open(sys.argv[1], 'r') as f:
        g = Graph(f)
        s = int(sys.argv[2])
        paths = DepthFirstPaths(g, s)
        for v in range(g.V()):
            print('%d to %d: ' % (s, v), end='')
            if paths.hasPathTo(v):
                for x in paths.pathTo(v):
                    if x == s:
                        print(x, end='')
                    else:
                        print('-%d' % (x), end='')
            print()

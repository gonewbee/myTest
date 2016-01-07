from __future__ import print_function
import sys

# This is not required if you've installed pycparser into
# your site-packages/ with setup.py
sys.path.extend(['.', '..'])

from pycparser import c_parser, c_ast, parse_file

class IfVisitor(c_ast.NodeVisitor):
    def __init__(self):
        pass

    # 查找if语句
    def visit_If(self, node):
        node.show()
        # 打印if语句出现位置
        print('if at %s' % (node.coord))
        left = node.cond.left
        if isinstance(left, c_ast.ID):
            print('left:%s' % (node.cond.left.name))
        elif isinstance(left, c_ast.Constant):
            print('left:%s' % (node.cond.left.value))
        # 遍历下一层，如果不加这句查不到嵌套的语句
        self.generic_visit(node);

    def visit_For(self, node):
        # node.show()
        print('if at %s' % (node.coord))
        self.generic_visit(node)


def start(filename):
    ast = parse_file(filename, use_cpp=True)
    v = IfVisitor()
    v.visit(ast)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    else:
        filename = 'test.c'

    start(filename)

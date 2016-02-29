class Edge():
    """带权重的边"""

    def __init__(self, v, w, weight):
        self.v = v
        self.w = w
        self.weightl = weight

    def weight(self):
        return self.weightl

    def either(self):
        return self.v

    def other(self, vertex):
        if vertex == self.v:
            return self.w
        elif vertex == self.w:
            return self.v
        else:
            raise Exception("Inconsistent edge")

    def compareTo(self, that):
        if not isinstance(that, Edge):
            print("that need be Edge")
            return -1
        if self.weightl < that.weightl:
            return -1
        elif self.weightl > that.weightl:
            return 1
        else:
            return 0

    def __repr__(self):
        s = str.format("%d-%d %.2f" % (self.v, self.w, self.weightl))
        return s

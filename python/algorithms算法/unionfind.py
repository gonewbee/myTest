"""
并查级(Union-Find)的基类
"""
class UF():
    
    def __init__(self, N):
        # 集合数量
        self.count = N
        self.id = list(range(N))
    
    def counts(self):
        return self.count
        
    def connected(self, p, q):
        """判断是否相连"""
        return self.find(p) == self.find(q)
    
    def find(self, p):
        """获取所属集合"""
        raise NotImplementedError( "Should have implemented this" )
    
    def union(self, p, q):
        """连接两个集合"""
        raise NotImplementedError( "Should have implemented this" )

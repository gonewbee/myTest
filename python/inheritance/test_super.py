'''
http://stackoverflow.com/questions/5033903/python-super-method-and-calling-alternatives
'''
class A(object):
    def __init__(self):
        print('Running A.__init__')
        super(A,self).__init__()
class B(A):
    def __init__(self):
        print('Running B.__init__')        
        # super(B,self).__init__()
        A.__init__(self) 

class C(A):
    def __init__(self):
        print('Running C.__init__')
        super(C,self).__init__()
class D(B,C):
    def __init__(self):
        print('Running D.__init__')
        super(D,self).__init__()

foo=D()

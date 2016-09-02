'''
参考: http://docs.cherrypy.org/en/latest/extend.html#tools
'''
import cherrypy

class Foo(object):
    @cherrypy.expose
    def index(self):
        print('Foo index enter\n')
        return 'Hello World\n'

def test_it():
    print('test_in enter\n')

if __name__ == '__main__':
    # 声明tools，hook point在handler之前
    cherrypy.tools.testit = cherrypy.Tool('before_handler', test_it)
    # 使用声明的tools，http://docs.cherrypy.org/en/latest/basics.html#perappconf
    cherrypy.quickstart(Foo(), '/', {'/': {'tools.testit.on': True}})

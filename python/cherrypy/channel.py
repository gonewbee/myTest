import cherrypy

class Foo(object):
    @cherrypy.expose
    def index(self):
        hello = 'Hello World!\n'
        cherrypy.engine.publish('index', hello)
        return hello

def test_index(d):
    print('test_index::%s' % d)

if __name__ == '__main__':
    cherrypy.engine.subscribe('index', test_index)
    cherrypy.quickstart(Foo())

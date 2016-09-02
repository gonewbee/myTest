import cherrypy

class Foo(object):
    @cherrypy.expose
    def index(self):
        return "Hello World Foo!\n"

class Bar(object):
    @cherrypy.expose
    def index(self):
        return "Hello World Bar!\n"

if __name__ == '__main__':
    cherrypy.tree.mount(Foo(), '/foo')
    cherrypy.tree.mount(Bar(), '/bar')
    cherrypy.engine.start()
    cherrypy.engine.block()

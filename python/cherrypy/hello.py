import random
import string

import cherrypy

class HelloWorld(object):
    @cherrypy.expose
    def index(self):
        return "Hello World!"

    @cherrypy.expose
    def generate(self, length=8):
        some_thing = ''.join(random.sample(string.hexdigits, int(length)))
        cherrypy.session['mystring'] = some_thing
        return some_thing

    @cherrypy.expose
    def display(self):
        return cherrypy.session['mystring']

if __name__ == '__main__':
    conf = {
        '/': {
            'tools.sessions.on': True
        }
    }
    cherrypy.quickstart(HelloWorld(), '/', conf)

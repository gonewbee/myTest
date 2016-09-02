'''
测试
    curl http://127.0.0.1:8080/nirvana/
    curl http://127.0.0.1:8080/nirvana/albums/nevermind/
    curl http://127.0.0.1:8080/nirvana/albums/nevermind/tracks/06/polly/
'''
import cherrypy

@cherrypy.popargs('name')
class Band(object):
    def __init__(self):
        self.albums = Album()

    @cherrypy.expose
    def index(self, name):
        return 'About %s...\n' % name

@cherrypy.popargs('title')
class Album(object):
    def __init__(self):
        self.tracks = Track()

    @cherrypy.expose
    def index(self, name, title):
        return 'About %s by %s...\n' % (title, name)

@cherrypy.popargs('num', 'track')
class Track(object):
    @cherrypy.expose
    def index(self, name, title, num, track):
        return 'About %s by %s... %s %s\n' % (title, name, num, track)

if __name__ == '__main__':
    cherrypy.quickstart(Band())

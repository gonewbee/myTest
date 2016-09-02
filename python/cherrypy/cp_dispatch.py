'''
测试:
    curl http://127.0.0.1:8080/nirvana/
    curl http://127.0.0.1:8080/nirvana/albums/nevermind/
'''
import cherrypy

class Band(object):
    def __init__(self):
        self.albums = Album()

    def _cp_dispatch(self, vpath):
        print("_cp_dispatch enter vpath:%s" % vpath)
        if len(vpath) == 1:
            cherrypy.request.params['name'] = vpath.pop()
            return self

        if len(vpath) == 3:
            cherrypy.request.params['artist'] = vpath.pop(0)
            vpath.pop(0)
            cherrypy.request.params['title'] = vpath.pop(0)
            return self.albums

        return vpath

    @cherrypy.expose
    def index(self, name):
        print('Band index enter')
        return 'About %s...\n' % name

class Album(object):
    @cherrypy.expose
    def index(self, artist, title):
        print('Album index enter')
        return 'About %s by %s...\n' % (title, artist)

if __name__ == '__main__':
    cherrypy.quickstart(Band())


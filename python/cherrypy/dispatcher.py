import random
import string

import cherrypy
from cherrypy._cpdispatch import Dispatcher

class StringGenerator(object):
    @cherrypy.expose
    def generate(self, length=8):
        print('generate enter')
        return ''.join(random.sample(string.hexdigits, int(length)))

class ForceLowerDispatcher(Dispatcher):
    def __call__(self, path_info):
        print('ForceLowerDispatcher path_info:%s' % path_info)
        return Dispatcher.__call__(self, path_info.lower())

if __name__ == '__main__':
    conf = {
        '/': {
            'request.dispatch': ForceLowerDispatcher(),
        }
    }
    cherrypy.quickstart(StringGenerator(), '/', conf)

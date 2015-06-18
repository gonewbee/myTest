#coding=utf-8
'''
    使用SocketServer建立tcp服务
'''
import SocketServer

class MyTCPHandler(SocketServer.BaseRequestHandler):
    """
    The RequestHandler class for our server.

    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """

    def handle(self):
        print("connect:%s" % self.client_address[0])
        # self.request is the TCP socket connected to the client
        while True:
            self.data = self.request.recv(1024).strip()
            if not self.data:
                print("recv not data")
                break
            print("{} wrote:{}".format(self.client_address[0], self.data))
            # just send back the same data, but upper-cased
            self.request.sendall(self.data.upper())

if __name__ == "__main__":
    HOST, PORT = "" , 12138

    # Create the server, binding to localhost on port 9999
    server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()

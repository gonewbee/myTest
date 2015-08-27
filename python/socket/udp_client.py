# coding=utf-8
import socket
addr = ('127.0.0.1', 12139)
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# "startApp:[\"C:\\Program Files\\Microsoft Office\\Office14\\WINWORD.Exe\"]"
# "startApp:[\\\\tsclient\\home\\home\\zsy\\lqbz\\test.doc]"#"startApp:[||CALC]"
# data = "startApp:[||WINWORD]"
# data = "startDoc:[C:\\Program Files\\Microsoft
# Office\\Office14\\WINWORD.EXE#\\\\tsclient\\home\\home\\zsy\\lqbz\\测试.doc]"
data = "startDoc:[||mass_samba#zsy \\\\192.168.104.27\\zsy Aa123!]"
s.sendto(data, addr)
data, ADDR = s.recvfrom(32)
print(data)
s.close()

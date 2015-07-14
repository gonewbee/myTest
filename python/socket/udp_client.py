import socket
addr=('127.0.0.1', 12139)
s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
data = "startApp:[||CALC]"#"startApp:[||WINWORD]"
s.sendto(data,addr)
data, ADDR = s.recvfrom(32)
print(data)
s.close()

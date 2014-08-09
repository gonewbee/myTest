'''
将jar文件隐写到png文件的tEXt段
'''
import sys
import binascii
fpng=open('test.png', 'rb')
fjar=open('test.jar', 'rb')
fout=open('out.png', 'wb')
dpng=fpng.read()
djar=fjar.read()
'''
#将jar数据放到png文件后
dout=bytearray(dpng)
dout+=bytearray(djar)
'''
index=8
ihdrLen=(dpng[index]<<24)+(dpng[index+1]<<16)+(dpng[index+2]<<8)+dpng[index+3]
index+=4
print(dpng[index:index+4])
if dpng[index:index+4] != b'IHDR':
    print("this is no ihdr")
    sys.exit(1)

index=index+ihdrLen+8
print(dpng[index:index+8])
dout=bytearray(dpng[0:index])
jarLen=len(djar)
dout+=bytearray([(jarLen>>24)&0xff, (jarLen>>16)&0xff, (jarLen>>8)&0xff, jarLen&0xff])
dout+=bytearray(b'tEXt')
crc=binascii.crc32(djar)
dout+=bytearray(djar)
dout+=bytearray([(crc>>24)&0xff, (crc>>16)&0xff, (crc>>8)&0xff, crc&0xff])
dout+=bytearray(dpng[index:])
fout.write(dout)
fpng.close()
fjar.close()
fout.close()

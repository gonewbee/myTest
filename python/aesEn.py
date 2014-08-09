'''
将文件用AES CBC模式加密
需要安装sudo pacman -S python-crypto
'''
from Crypto.Cipher import AES
from Crypto import Random

f = open('test.png', 'rb')
fen = open('testEn.png', 'wb')
key = b'Sixteen byte key'
iv = Random.new().read(AES.block_size)
cipher = AES.new(key, AES.MODE_CBC, iv)
text = f.read()
count = len(text)
add = count%16
print("len:{1:d};add:{0:d}".format(add, count))
if add != 0:
    dtmp = bytearray(text)
    add = 16-add
    dtmp += str.encode('\0'*add)
    text=bytes(dtmp)
dout = iv+cipher.encrypt(text)
fen.write(dout)
f.close()
fen.close()

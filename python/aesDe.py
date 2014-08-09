'''
将文件用AES CBC模式解密
需要安装sudo pacman -S python-crypto
'''
from Crypto.Cipher import AES
from Crypto import Random

f = open('testEn.png', 'rb')
fde = open('testDe.png', 'wb')
key = b'Sixteen byte key'
iv = Random.new().read(AES.block_size)
cipher = AES.new(key, AES.MODE_CBC, iv)
text = f.read()
dtmp = cipher.decrypt(text)
dout = bytearray(dtmp)
while dout[-1]==0:
    dout=dout[:-1]
dout=dout[16:]
fde.write(dout)
f.close()
fde.close()

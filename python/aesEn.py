'''
将文件用AES CBC模式加密，前四个字节是文件长度
需要安装sudo pacman -S python-crypto
'''
from Crypto.Cipher import AES
from Crypto import Random

def encode_aes(fileIn, fileOut):
    f = open(fileIn, 'rb')
    fen = open(fileOut, 'wb')
    key = b'Sixteen byte key'
    iv = Random.new().read(AES.block_size)
    cipher = AES.new(key, AES.MODE_CBC, iv)
    text = f.read()
    count = len(text)
    add = (count+4)%16
    print("len:{1:d};add:{0:d}".format(add, count))
    dtmp = bytearray([(count>>24)&0xff, (count>>16)&0xff, (count>>8)&0xff, count&0xff])
    dtmp += bytearray(text)
    if add != 0:
        add = 16-add
        dtmp += str.encode('\0'*add)
    
    text=bytes(dtmp)
    dout = iv+cipher.encrypt(text)
    fen.write(dout)
    f.close()
    fen.close()

if __name__=="__main__":
    encode_aes('test.png', 'testEn.png')

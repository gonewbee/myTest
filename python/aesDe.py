'''
将文件用AES CBC模式解密，前四个字节是文件长度
需要安装sudo pacman -S python-crypto
'''
from Crypto.Cipher import AES
from Crypto import Random
def decode_aes(fileIn, fileOut):
    f = open(fileIn, 'rb')
    fde = open(fileOut, 'wb')
    key = b'Sixteen byte key'
    iv = Random.new().read(AES.block_size)
    cipher = AES.new(key, AES.MODE_CBC, iv)
    text = f.read()
    dtmp = cipher.decrypt(text)
    dout = bytearray(dtmp)
    fileLen = ((dout[16]&0xff)<<24)+((dout[17]&0xff)<<16)+((dout[18]&0xff)<<8)+(dout[19]&0xff)
    if (fileLen+20>len(dout)):
        dout=dout[20:]
    else:
        dout=dout[20:fileLen+20]
    fde.write(dout)
    f.close()
    fde.close()

if __name__=="__main__":
    decode_aes('testEn.png', 'testDe.png')

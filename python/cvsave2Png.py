'''
将文件通过AES加密，然后保存入png文件
通过ctyps调用编写的使用opencv的so库，将文件保存到png的第四通道
传递char *类型时使用c_char_p(b"test.png")
'''
import aesEn
from ctypes import *

aesEn.encode_aes('test.jar', 'testEn.jar')  #将jar文件加密
rpng = cdll.LoadLibrary("/home/zsy/lqbz/java/librpng.so")
rpng.savejar2png(c_char_p(b"testEn.jar"), c_char_p(b"test.png"), c_char_p(b"cvout.png"))

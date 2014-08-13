'''
将文件从png中提取出来并通过AES解密
通过ctyps调用编写的使用opencv的so库，文件保存在png的第四通道
传递char *类型时使用c_char_p(b"test.png")
'''
import aesDe
from ctypes import *

rpng = cdll.LoadLibrary("/home/zsy/lqbz/java/librpng.so")
rpng.getfrompng(c_char_p(b"cvout.png"), c_char_p(b"frompng.jar"))
aesDe.decode_aes("frompng.jar", "final.jar")

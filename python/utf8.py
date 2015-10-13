#!/usr/bin/python3
# 测试utf8字符串转bytes
# 参考http://stackoverflow.com/questions/7585435/best-way-to-convert-string-to-bytes-in-python-3

s = "失败"
print(s)
b = s.encode("utf-8")
print(b)

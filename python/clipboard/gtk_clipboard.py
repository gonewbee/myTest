# coding=utf-8
"""
使用pygtk获取剪切板中数据
"""
import gtk

clipboard = gtk.clipboard_get()
# 打印剪切板中内容
print(clipboard.wait_for_text())

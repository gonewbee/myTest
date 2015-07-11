#coding=utf-8
'''
python关于文件路径的操作
'''
import os

home_path = os.path.expanduser("~") #获取用户根目录
print(home_path)

file_path = os.path.abspath(__file__) #文件的绝对路径
print(file_path)

parent_path = os.path.dirname(file_path) #获取file_path的父目录
print(parent_path)

current_path = os.getcwd() #当前路径
print(current_path)


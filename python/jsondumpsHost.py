'''
生成json文件，内容为apk升级描述

python jsondumpsHost.py -s 192.168.11.84 -f test.apk -n 1 -n 2 -v 4 -p
'''

import argparse
import json
import hashlib

def debugPrint(level, megs):
    if level:
        print(megs)

def parseArgs():
    parser = argparse.ArgumentParser(description="apk升级描述信息")
    parser.add_argument('-s', '--host', dest='host', help='服务端')
    parser.add_argument('-v', '--verCode', dest='versionCode', type=int,  help='apk版本号')
    parser.add_argument('-m', '--md5', dest='md5', help='apk的md5值')
    #action='append' 将多个--noNeed组合成list
    parser.add_argument('-n', '--noNeed', dest='noNeed', action='append',
            type=int, help='不需要升级的版本')
    parser.add_argument('-o', '--out', dest='output', default='apkinfo.json',
            help='输出文件')
    parser.add_argument('-f', '--file', dest='apkfile', help='apk文件')
    #默认enprint为False
    parser.add_argument('-p', '--print', action='store_true', dest='enprint',help='显示打印')
    #分析命令行参数
    argRet = parser.parse_args()
    mData = dict()
    if argRet.host!=None:
        mData["jsonURL"] = 'http://' + argRet.host + '/files/' + argRet.output
    if argRet.versionCode != None:
        mData["versionCode"] = argRet.versionCode
    if argRet.md5 != None:
        mData["md5"] = argRet.md5s
    if argRet.noNeed != None:
        mData["noNeed"] = argRet.noNeed
    if argRet.apkfile != None:
        mData["apkURL"] = 'http://' + argRet.host + '/files/' + argRet.apkfile
        fin = open(argRet.apkfile, 'rb')
        md5l=hashlib.md5(fin.read()).hexdigest()
        debugPrint(argRet.enprint, "md5: " + md5l)
        mData["md5"] = md5l
    debugPrint(argRet.enprint, mData)

    #将apk信息转成json并保存到output中
    dataJson = json.dumps(mData)
    fout = open(argRet.output, 'w');
    fout.write(dataJson)
    fout.close()

if __name__ == "__main__":
    parseArgs()


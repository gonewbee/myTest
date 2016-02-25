#! coding=utf-8
import numpy as np
import cv2
import argparse
import os
import sys

# rgb565转bgr888，opencv排序是bgr
# 参考http://stackoverflow.com/questions/2442576/how-does-one-convert-16-bit-rgb565-to-24-bit-rgb888


def rgb565tobgr888(h, l):
    R5 = (h >> 3) & 0x1f
    G6 = ((h & 0x7) << 3) + ((l >> 5) & 0x7)
    B5 = l & 0x1f
    R8 = (R5 * 527 + 23) >> 6
    G8 = (G6 * 259 + 33) >> 6
    B8 = (B5 * 527 + 23) >> 6
    return (B8, G8, R8)


def argb4444tobgr888(h, l):
    R4 = h & 0xf
    G4 = (l & 0xf0) >> 4
    B4 = l & 0xf
    R8 = R4 * 15
    G8 = G4 * 15
    B8 = B4 * 15
    return (B8, G8, R8)

# rgb565是16bit，由于是小端模式，要调整这两个字节的高低位


def getImage(fname, argb4444=False, w=1024, h=20, d=3):
    img = np.zeros((h, w, d), np.uint8)
    f = open(fname, 'rb')
    data = f.read()

    k = 0
    for i in range(h):
        for j in range(w):
            l = ord(data[k])
            h = ord(data[k + 1])
            k += 2
            if argb4444:
                img[i, j] = argb4444tobgr888(h, l)
            else:
                img[i, j] = rgb565tobgr888(h, l)
    f.close()
    return img

if __name__ == "__main__":
    input = 'dump.dex'
    w = 1024
    h = 15
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-s', '--save', action='store_true', help='save to file')
    parser.add_argument('--argb4444', action='store_true', help='save to file')
    parser.add_argument(
        '-l', '--loop', action='store_true', help='loop to detect width')
    parser.add_argument('-i', '--input', help='input file name')
    parser.add_argument('-o', '--output', help='output file name')
    parser.add_argument('-w', '--width', type=int, help='image width')
    parser.add_argument('-t', '--height', type=int, help='image height')
    args = parser.parse_args()
    print(args)
    if args.input is not None:
        input = args.input
    if args.width is not None:
        w = args.width
    if args.height is not None:
        h = args.height
    print("Input file:%s" % (input))
    if args.loop:
        directory = 'tmp/'
        # 如果目录不存在创建目录
        if not os.path.exists(directory):
            os.makedirs(directory)
        h = 15
        for w in range(1200, 1280):
            print('w::%d' % (w))
            img = getImage(input, args.argb4444, w, h)
            out = 'tmp/' + str(w) + '.png'
            cv2.imwrite(out, img)
        sys.exit(0)
    img = getImage(input, args.argb4444, w, h)
    if args.save:
        if args.output is not None:
            cv2.imwrite(args.output, img)
        else:
            cv2.imwrite('test.png', img)
    else:
        cv2.imshow('image', img)
        cv2.waitKey(0)
        cv2.destroyAllWindows()

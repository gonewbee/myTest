# coding=utf-8
import numpy as np
import cv2

# Create a black image
img = np.zeros((512, 512, 3), np.uint8)

# 按像素点操作，设置像素点数值
for i in range(100, 200):
    for j in range(100, 150):
        # 按照BGR顺序存储
        img[i, j] = [0, 0, 255]


cv2.imshow('image', img)
cv2.waitKey(0)
cv2.destroyAllWindows()

package com.ksh.pngops.deals;

import android.graphics.Bitmap;


public interface IPixelDetail {
	public Bitmap saveBuf2Bitmap(byte[] fileBuf, Bitmap bmp);
	/**
	 * 判断png图片像素点是否足够
	 * @param len 要隐写的文件的长度
	 * @param width png宽
	 * @param height png高
	 * @return true：png像素足够 false：png像素点少
	 */
	public boolean checkLen(int len, int width, int height);
	
	public boolean isFileIn(Bitmap bmp);
	
	/**
	 * 返回png中隐写的文件内容，前四个字节是文件长度
	 * @param bmp png 对应的Bitmap
	 * @return png中隐写的文件内容，前四个字节是文件长度; null是不保护内容
	 */
	public byte[] getFileBufFromPng(Bitmap bmp);
}

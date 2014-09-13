package com.ksh.pngops;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import com.ksh.pngops.deals.IPixelDetail;
import com.ksh.pngops.deals.PixelDetail2alpha;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Bitmap.Config;
import android.util.Log;

public class PngHandler {
	public static final int SAVE2ALPHALOW = 0x11;
	private final String TAG = "Save2png";
	
	IPixelDetail pixelDetail = new PixelDetail2alpha();
	
	public PngHandler(int type) {
		switch (type) {
		case SAVE2ALPHALOW:
			pixelDetail = new PixelDetail2alpha();
			break;
		}
	}
	
	/**
	 * 将文件保存到png的alpha通道
	 * 
	 * 前四个字节是文件长度，每个源数据保存到alpha通道的低四位
	 * @param srcStr 要保存的源文件
	 * @param pngStr 保存到的文件
	 */
	public boolean savefile2png(String srcStr, String pngStr) {
		if (!pngStr.endsWith(".png")) {
			Log.e(TAG, "input is not png");
			return false;
		}
		try {
			Bitmap bmp = BitmapFactory.decodeFile(pngStr);
			if (bmp==null) {
				Log.e(TAG, "input png data is null");
				return false;
			}
			FileInputStream fin = new FileInputStream(srcStr);
			int fileLen = fin.available();
			int width = bmp.getWidth();
			int height = bmp.getHeight();
			if (!pixelDetail.checkLen(fileLen, width, height)){
				Log.e(TAG, "png file is small fileLen:" + fileLen + ";;png: " + width + "*" + height);
				return false;
			}
			byte[] fileBuf = new byte[fileLen+4];
			fileBuf[0] = (byte) ((fileLen>>24)&0xff);
			fileBuf[1] = (byte) ((fileLen>>16)&0xff);
			fileBuf[2] = (byte) ((fileLen>>8)&0xff);
			fileBuf[3] = (byte) (fileLen&0xff);
			fin.read(fileBuf, 4, fileLen);
			Bitmap bmpout = pixelDetail.saveBuf2Bitmap(fileBuf, bmp);
			String outStr = pngStr.substring(0, pngStr.length()-4) + "_out" + pngStr.substring(pngStr.length()-4);
			Log.d(TAG, "output file: " + outStr);
			FileOutputStream fout = new FileOutputStream(outStr);
			bmpout.compress(Bitmap.CompressFormat.PNG, 100, fout);
			fin.close();
			fout.close();
			return true;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	public boolean getFileFromPng(String pngStr) {
		if (!pngStr.endsWith(".png")) {
			Log.e(TAG, "get file from png error!!, input is not png file");
			return false;
		}
		Bitmap bmp = BitmapFactory.decodeFile(pngStr);
		if (bmp==null) {
			Log.e(TAG, "get file from png error!!, input png file data is null");
			return false;
		}
		byte[] buf = pixelDetail.getFileBufFromPng(bmp);
		if (buf==null)
			return false;
		String outStr = pngStr.substring(0, pngStr.length()-4) + "_get";
		Log.d(TAG, "output file: " + outStr);
		try {
			FileOutputStream fout = new FileOutputStream(outStr);
			fout.write(buf, 4, buf.length-4);
			fout.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}
}

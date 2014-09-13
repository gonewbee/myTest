package com.ksh.pngops.deals;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Bitmap.Config;
import android.util.Log;

public class PixelDetail2alpha implements IPixelDetail {
	private final String TAG = "PixelDetail2alpha";
	@Override
	public boolean checkLen(int fileLen, int width, int height) {
		if (((fileLen+4)*2)>(width*height))
			return false;
		return true;
	}

	@Override
	public Bitmap saveBuf2Bitmap(byte[] fileBuf, Bitmap bmp) {
		int width = bmp.getWidth();
		int height = bmp.getHeight();
		Bitmap bmpout = Bitmap.createBitmap(width, height, Config.ARGB_8888);
		int bufLen = fileBuf.length*2;
		int index = 0;
		int i, j;
		int cABGR = 0;
		//遍历顺序，先行再列
		for (j=0; j<height; j++) {
			for (i=0; i<width; i++) {
				cABGR = bmp.getPixel(i, j);
				if (index<bufLen) {
					cABGR = saveData2alphaL(fileBuf[index/2], index, cABGR);
					if (index<8)
						System.out.printf("%x\n", cABGR);
					index++;
				}
				
				bmpout.setPixel(i, j, cABGR);
			}
		}
		return bmpout;
	}

	@Override
	public boolean isFileIn(Bitmap bmp) {
		int width = bmp.getWidth();
		int height = bmp.getHeight();
		int dstFileLen = getFileLenFromPng(bmp);
		if (dstFileLen==0)
			return false;
		if (((dstFileLen+4)*2)>(width*height))
			return false;
		return true;
	}

	public byte[] getFileBufFromPng(Bitmap bmp) {
		int width = bmp.getWidth();
		int height = bmp.getHeight();
		int fileLen = getFileLenFromPng(bmp);
		Log.d(TAG, "get file from png len: " + fileLen);
		if (fileLen==0)
			return null;
		if (((fileLen+4)*2)>(width*height))
			return null;
		int bufLen = (fileLen+4)*2;
		byte[] buf = new byte[fileLen+4];
		int i, j;
		int readLen = 0;
		for (j=0; j<height; j++) {
			for (i=0; i<width; i++) {
				int cRAGB = bmp.getPixel(i, j);
				getDataFromPixels(buf, readLen, cRAGB);
				readLen++;
				if (readLen>=bufLen)
					break;
			}
			if (readLen>=bufLen)
				break;
		}
		return buf;
	}
	private int saveData2alphaL(byte data, int index, int cABGR) {
		int a;
		if (index%2==0)
			a = ((data&0xf0)>>4) | (Color.alpha(cABGR)&0xf0);
		else
			a = (data&0x0f) | (Color.alpha(cABGR)&0xf0);
		return Color.argb(a, Color.red(cABGR), Color.green(cABGR), Color.blue(cABGR));
	}
	
	private int getFileLenFromPng(Bitmap bmp) {
		int width = bmp.getWidth();
		int height = bmp.getHeight();
		if (width*height<=8)
			return 0;
		int i, j;
		int readLen = 0;
		byte[] buf = new byte[4];
		for (j=0; j<height; j++) {
			for (i=0; i<width; i++) {
				int cRAGB = bmp.getPixel(i, j);
				getDataFromPixels(buf, readLen, cRAGB);
				System.out.printf("cRAGB:%x, alpha:%x\n", cRAGB, Color.alpha(cRAGB));
				System.out.printf("buf[%d]:%x\n", readLen/2, buf[readLen/2]);
				readLen++;
				if (readLen>=8)
					break;
			}
			if (readLen>=8)
				break;
		}
		int dstFileLen = ((buf[0]&0xff)<<24)|((buf[1]&0xff)<<16)|((buf[2]&0xff)<<8)|(buf[3]&0xff);
		return dstFileLen;
	}
	
	private void getDataFromPixels(byte[] buf, int index, int cRAGB) {
		int data = Color.alpha(cRAGB)&0x0f;
		int rIndex = index/2;
		if (index%2==0) {
			buf[rIndex] = (byte) ((data<<4)|(buf[rIndex]&0x0f));
		} else {
			buf[rIndex] = (byte) (data|(buf[rIndex]&0xf0));
		}
	}
}

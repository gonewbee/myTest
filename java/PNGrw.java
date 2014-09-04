/**
 * 将数据分别保存到ARGB的低两位
 */
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import javax.imageio.ImageIO;

public class PNGrw {

	public static void main(String[] args) {
		switch (args.length) {
		case 0:
			logHelp();
			break;
		case 1:
			if (args[0].equalsIgnoreCase("-h")||args[0].equalsIgnoreCase("--help")) 
				logHelp(); 
			else
				System.out.println("输入格式错误");
			break;
		case 2:
			if (args[0].equalsIgnoreCase("-g")||(args[0].equalsIgnoreCase("--get"))) {
				if (!args[1].endsWith(".png")) {
					System.out.println("输入不是png文件");
					break;
				}
				File fin = new File(args[1]);
				if (!fin.exists()) {
					System.out.println("文件不存在");
					break;
				}
				String fileName = fin.getName();
				String outName = "getFrom_"+fileName.substring(0, fileName.length()-4);
				System.out.println("outName: " + outName);
				getFileFromPNG(args[1], outName);
			} else {
				System.out.println("输入格式错误");
			}
			break;
		case 3:
			if (args[0].equalsIgnoreCase("-s")||(args[0].equalsIgnoreCase("--save"))) {
				if (!args[2].endsWith(".png")) {
					System.out.println("输入不是png文件");
					break;
				}
				File fin0 = new File(args[1]);
				File fin = new File(args[2]);
				if (!fin.exists()||!fin0.exists()) {
					System.out.println("输入文件不存在");
					break;
				}
				String fileName = fin.getName();
				String outName = fileName.substring(0, fileName.length()-4)+"_out"+fileName.substring(fileName.length()-4);
				System.out.println("outName: " + outName);
				saveFile2PNG(args[2], args[1], outName);
			} else {
				System.out.println("输入格式错误");
			}
			break;
		default:
			logHelp();
		}
	}
	
	public static void saveFile2PNG(String inImage, String inFile, String outImage) {
		File fInput = new File(inImage);
		try {
			BufferedImage image = ImageIO.read(fInput);
			int width = image.getWidth();
			int height = image.getHeight();
//			if (image.getType()!=BufferedImage.TYPE_4BYTE_ABGR) {
//				System.out.println("该png图像不是TYPE_4BYTE_ABGR格式");
//				return;
//			}
			int i, j;
			int cABGR = 0;
			int alpha;
			FileInputStream fin = new FileInputStream(inFile);
			int len = fin.available();
			System.out.printf("len:%d; image:%d\n", len, width*height);
			if ((len+4)>width*height) {
				System.out.println("png file is small");
				return;
			}
			byte[] fileBuf = new byte[len+4];
			fileBuf[0] = (byte) ((len>>24)&0xff);
			fileBuf[1] = (byte) ((len>>16)&0xff);
			fileBuf[2] = (byte) ((len>>8)&0xff);
			fileBuf[3] = (byte) (len&0xff);
			fin.read(fileBuf, 4, len);
			len+=4;	//前四个字节为文件长度
			int index = 0;
			BufferedImage imageout = new BufferedImage(width, height, BufferedImage.TYPE_4BYTE_ABGR);
			//遍历顺序，先行再列
			for (j=0; j<height; j++) {
				for (i=0; i<width; i++) {
					cABGR = image.getRGB(i, j);
					if (index<len) {
						cABGR = saveData2Pixel(fileBuf[index], cABGR);
						index++;
					}
					imageout.setRGB(i, j, cABGR);
				}
			}
			File outputfile = new File(outImage);
			ImageIO.write(imageout, "png", outputfile);
			fin.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public static void getFileFromPNG(String pngFile, String fileout) {
		File fInput = new File(pngFile);
		BufferedImage image;
		try {
			image = ImageIO.read(fInput);
			int width = image.getWidth();
			int height = image.getHeight();
//			if (image.getType()!=BufferedImage.TYPE_4BYTE_ABGR) {
//				System.out.println("该png图像不是TYPE_4BYTE_ABGR格式");
//				return;
//			}
			int len = getFileLen(image);
			System.out.printf("len:%d\n", len);
			len+=4;
			byte[] buf = new byte[len];
			int i, j;
			int index=0;
			int cABGR;
			//遍历顺序，先行再列
			for (j=0; j<height; j++) {
				for (i=0; i<width; i++) {
					cABGR = image.getRGB(i, j);
					if (index>=len)
						break;
					buf[index] = getDataFromPixel(cABGR);
					index++;
				}
			}
			FileOutputStream fout = new FileOutputStream(fileout);
			fout.write(buf, 4, len-4);
			fout.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * 前四个字节保存的是文件的长度
	 * @param image
	 * @return
	 */
	private static int getFileLen(BufferedImage image) {
		int cABGR = image.getRGB(0, 0);
		byte data = getDataFromPixel(cABGR);
		int len = (data&0xff)<<24;
		cABGR = image.getRGB(1, 0);
		data = getDataFromPixel(cABGR);
		len += ((data&0xff)<<16);
		cABGR = image.getRGB(2, 0);
		data = getDataFromPixel(cABGR);
		len += ((data&0xff)<<8);
		cABGR = image.getRGB(3, 0);
		data = getDataFromPixel(cABGR);
		len += (data&0xff);
		return len;
	}
	
	/**
	 * 将数据保存如pixel
	 * @param data 要保存的数据
	 * @param srcData 该像素的的原始数据，argb顺序，数据分别存储在BGRA
	 * @return
	 */
	private static int saveData2Pixel(byte data, int srcData) {
		int b = (data&0xc0)>>6;
		int g = (data&0x30)>>4;
		int r = (data&0x0c)>>2;
		int a = data&0x03;
		int dst = (a<<24)|(r<<16)|(g<<8)|b|(srcData&0xfcfcfcfc);
		return dst;
	}
	
	/**
	 * 从pixel中提取数据，数据顺序为BGRA
	 * @param srcData 顺序为argb
	 * @return
	 */
	private static byte getDataFromPixel(int srcData) {
		byte a = (byte) ((srcData>>24)&0x03);
		byte r = (byte) ((srcData>>16)&0x03);
		byte g = (byte) ((srcData>>8)&0x03);
		byte b = (byte) (srcData&0x03);
		byte ret = (byte) ((b<<6)|(g<<4)|(r<<2)|a);
		return ret;
	}
	
	private static void logHelp() {
		System.out.println("-h --help: 帮助");
		System.out.println("-s --save: file image.png 将file隐藏到image.png");
		System.out.println("-g --get: image.png 从image.png中提取文件");
	}
}

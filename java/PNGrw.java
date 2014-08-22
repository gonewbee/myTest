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
		String inImage = "/home/zsy/data/A20/NgbCard/res/drawable/icon.png";
		String inFile = "/home/zsy/workspace/myTest/DexClassLoaderTest/usedex/libs/armeabi/libgetso.so";
		saveFile2PNG(inImage, inFile, "out.png");
		getFileFromPNG("out.png", "out.so");
	}
	
	public static void saveFile2PNG(String inImage, String inFile, String outImage) {
		File fInput = new File(inImage);
		try {
			BufferedImage image = ImageIO.read(fInput);
			int width = image.getWidth();
			int height = image.getHeight();
			if (image.getType()==BufferedImage.TYPE_4BYTE_ABGR)
				System.out.println("has alpha");
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
			for (i=0; i<width; i++) {
				for (j=0; j<height; j++) {
					cABGR = image.getRGB(i, j);
					if (index<len) {
						cABGR = ((fileBuf[index]&0xc0)<<18)|((fileBuf[index]&0x30)<<12)|((fileBuf[index]&0x0c)<<6)|
								(fileBuf[index]&0x03)|(cABGR&0xfcfcfcfc);
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
			if (image.getType()==BufferedImage.TYPE_4BYTE_ABGR)
				System.out.println("has alpha");
			int len = getFileLen(image);
			System.out.printf("len:%d\n", len);
			len+=4;
			byte[] buf = new byte[len];
			int i, j;
			int index=0;
			int cABGR;
			for (i=0; i<width; i++) {
				for (j=0; j<height; j++) {
					cABGR = image.getRGB(i, j);
					if (index>=len)
						break;
					buf[index] = (byte) (((cABGR>>18)&0xc0)|((cABGR>>12)&0x30)|((cABGR>>6)&0x0c)|(cABGR&0x03));
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
		int data = ((cABGR>>18)&0xc0)|((cABGR>>12)&0x30)|((cABGR>>6)&0x0c)|(cABGR&0x03);
		int len = (data&0xff)<<24;
		cABGR = image.getRGB(0, 1);
		data = ((cABGR>>18)&0xc0)|((cABGR>>12)&0x30)|((cABGR>>6)&0x0c)|(cABGR&0x03);
		len += ((data&0xff)<<16);
		cABGR = image.getRGB(0, 2);
		data = ((cABGR>>18)&0xc0)|((cABGR>>12)&0x30)|((cABGR>>6)&0x0c)|(cABGR&0x03);
		len += ((data&0xff)<<8);
		cABGR = image.getRGB(0, 3);
		data = ((cABGR>>18)&0xc0)|((cABGR>>12)&0x30)|((cABGR>>6)&0x0c)|(cABGR&0x03);
		len += (data&0xff);
		return len;
	}
	
}

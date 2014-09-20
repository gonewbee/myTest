/**
 * 计算从命令行输入的文件的md5值
 */

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Md5test {

	public static void main(String[] args) {
		if (args.length==0) {
			System.out.println("计算命令行输入文件的md5值，输入为空");
			return;
		}
		File file = new File(args[0]);
		if (!file.exists()) {
			System.out.println(args[0] + " not exists");
			return;
		}
		try {
			FileInputStream fis = new FileInputStream(file);
			int len = fis.available();
			byte[] buf = new byte[len];
			int readLen = fis.read(buf);
			MessageDigest md5 = MessageDigest.getInstance("MD5");
			md5.update(buf, 0, readLen);
			byte[] ret = md5.digest();
			System.out.println("md5: " + byte2hexString(ret));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}
	}
	
	private static String byte2hexString(byte[] buf) {
		int i;
		int len = buf.length;
		String str="";
		for (i=0; i<len; i++) {
			str += String.format("%02x", buf[i]);
		}
		return str;
	}

}

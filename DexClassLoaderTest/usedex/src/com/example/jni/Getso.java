package com.example.jni;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.content.Context;
import android.os.Environment;

public class Getso {
//	static {
//		System.loadLibrary("getso");
//	}
	private static Getso instance = null;
	public static Getso getInstance(Context cxt) {
		if (instance==null)
			instance = new Getso(cxt);
		return instance;
	}
	private Getso(Context cxt) {
		System.out.println("Getso create!!!!!!!!!");
		saveso2files(cxt);
		String soPath = cxt.getFilesDir() + File.separator + "libgetso.so";
		System.load(soPath);
		new File(soPath).delete();
	}
	public native String getVersion();
	public native boolean getBuf(byte[] in, byte[] out, byte[] key, int len);

	private boolean saveso2files(Context cxt) {
		String filePath = Environment.getExternalStorageDirectory().toString() + File.separator + "libgetso.so";
		try {
			FileInputStream in = new FileInputStream(filePath);
			FileOutputStream out = cxt.openFileOutput("libgetso.so", cxt.MODE_PRIVATE);
			int len = 0;
			byte[] data = new byte[1024];
			while ((len=in.read(data))>0) {
				out.write(data, 0, len);
			}
			in.close();
			out.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return true;
	}
}

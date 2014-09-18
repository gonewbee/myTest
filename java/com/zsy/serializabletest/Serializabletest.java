package com.zsy.serializabletest;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class Serializabletest {
	private String outStr = "data.out";
	private Config mConfig = null;
	public static void main(String[] args) {
		new Serializabletest().testSerializable();
	}
	
	public void testSerializable() {
		File fileOut = new File(outStr);
		if (fileOut.exists()) {
			try {
				FileInputStream fis = new FileInputStream(fileOut);
				ObjectInputStream ois = new ObjectInputStream(fis);
				mConfig = (Config) ois.readObject();
				ois.close();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			}
		} else {
			mConfig = new Config();
		}
		int ret = mConfig.addTimes(100);
		System.out.println("ret: " + ret);
		mConfig.ptimes += ret;
		System.out.println("ptimes: " + mConfig.ptimes);
		
		try {
			FileOutputStream fos = new FileOutputStream(fileOut);
			ObjectOutputStream oos = new ObjectOutputStream(fos);
			oos.writeObject(mConfig);
			oos.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}

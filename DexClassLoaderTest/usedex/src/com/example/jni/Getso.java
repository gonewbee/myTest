package com.example.jni;

public class Getso {
	static {
		System.loadLibrary("getso");
	}
	public native String getVersion();
	public native boolean getBuf(byte[] in, byte[] out, byte[] key);
}

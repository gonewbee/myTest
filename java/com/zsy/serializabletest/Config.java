package com.zsy.serializabletest;

import java.io.Serializable;

public class Config implements Serializable{
	private int times = 0;
	public int ptimes = 0;
	public int addTimes(int num) {
		times += num;
		return times;
	}
}

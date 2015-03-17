package com.zsy.explorer;

import java.io.File;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

public class MyService extends Service {

	private final IBinder mBinder = new LocalBinder();
	
	public String splitString(String str) {
		File f = new File(str);
		String name = f.getName();
		Log.d(FindUsbActivity.TAG, "name:" + name);
		return name;
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return mBinder;
	}
	
	public class LocalBinder extends Binder {
		MyService getService() {
			return MyService.this;
		}
	}
}

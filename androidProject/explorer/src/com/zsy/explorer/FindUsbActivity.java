package com.zsy.explorer;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.zsy.explorer.MyService.LocalBinder;

import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.os.Build;

public class FindUsbActivity extends Activity implements View.OnClickListener{

	public static final String TAG = "FindUsbActivity";
	private static final int REQUEST_EX = 1;
	private Button btn_start;
	private MyService mService = null;
	
	private ServiceConnection mConnection = new ServiceConnection() {

		@Override
		public void onServiceConnected(ComponentName arg0, IBinder service) {
			// TODO Auto-generated method stub
			Log.d(TAG, "onServiceConnected called");
			LocalBinder binder = (LocalBinder) service;
			mService = binder.getService();
		}

		@Override
		public void onServiceDisconnected(ComponentName arg0) {
			// TODO Auto-generated method stub
			mService = null;
		}
		
	};
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_find_usb);
		btn_start = (Button) findViewById(R.id.btn_explorer);
		btn_start.setOnClickListener(this);
	}
	
	

	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		Intent intent = new Intent(this, MyService.class);
		bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
	}



	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
	}



	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.find_usb, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.btn_explorer:
			ArrayList<String> list = GetUDisks.getUDiskDirs();
			Intent intent = new Intent();
			intent.putExtra("explorer_title",
					getString(R.string.dialog_read_from_dir));
			intent.putStringArrayListExtra("list", list);
			intent.setClass(FindUsbActivity.this, ExDialog.class);
			startActivityForResult(intent, REQUEST_EX);
			break;
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		if (resultCode == RESULT_OK) {
			if (requestCode == REQUEST_EX) {
				Bundle bl = data.getExtras();
				String path = bl.getString("results");
				System.out.println("onActivityResult path: " + path);
				if (mService!=null) {
					String name = mService.splitString(path);
					Log.d(TAG, "in onActivityResult file name: " + name);
				}
			}
		}
		super.onActivityResult(requestCode, resultCode, data);
	}
	
	

}

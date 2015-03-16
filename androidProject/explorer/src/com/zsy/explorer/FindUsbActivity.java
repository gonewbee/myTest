package com.zsy.explorer;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.os.Build;

public class FindUsbActivity extends Activity implements View.OnClickListener{

	private static final int REQUEST_EX = 1;
	private Button btn_start;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_find_usb);
		btn_start = (Button) findViewById(R.id.btn_explorer);
		btn_start.setOnClickListener(this);
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
			}
		}
		super.onActivityResult(requestCode, resultCode, data);
	}
	
	

}

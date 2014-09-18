package com.ksh.write2png;

import com.ksh.update.CheckUpdateTask;

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

public class Write2png extends Activity implements View.OnClickListener {

	private Button btn_write;
	private Button btn_read;
	private Button btn_update;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_write2png);
		btn_write = (Button) findViewById(R.id.btn_write);
		btn_read = (Button) findViewById(R.id.btn_read);
		btn_update = (Button) findViewById(R.id.btn_check_update);
		btn_write.setOnClickListener(this);
		btn_read.setOnClickListener(this);
		btn_update.setOnClickListener(this);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.write2png, menu);
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
	public void onClick(View arg0) {
		Intent intent = new Intent();
		switch (arg0.getId()) {
		case R.id.btn_write:
			intent.setClass(this, Writefile.class);
			startActivity(intent);
			break;
		case R.id.btn_read:
			intent.setClass(this, Readfile.class);
			startActivity(intent);
			break;
		case R.id.btn_check_update:
			new CheckUpdateTask(this).execute();
			break;
		}
	}

}

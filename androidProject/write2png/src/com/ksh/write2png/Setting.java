package com.ksh.write2png;

import com.ksh.update.CheckUpdateTask;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Setting extends Activity implements View.OnClickListener {

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.setting);
		String mversionName = "0";
		PackageManager manager = getPackageManager();
		try {
			PackageInfo info = manager.getPackageInfo(getPackageName(), 0);
			mversionName = info.versionName;
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		TextView txt_versionName = (TextView) findViewById(R.id.txt_ver_name);
		txt_versionName.setText(mversionName);
		Button btn_check = (Button) findViewById(R.id.btn_check_update);
		btn_check.setOnClickListener(this);
	}

	public void onClick(View arg0) {
		Intent intent = new Intent();
		switch (arg0.getId()) {
		case R.id.btn_check_update:
			new CheckUpdateTask(this).execute();
			break;
		}
	}

}

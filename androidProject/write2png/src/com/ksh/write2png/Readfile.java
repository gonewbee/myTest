package com.ksh.write2png;

import java.io.File;

import com.ksh.pngops.PngHandler;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class Readfile  extends Activity implements View.OnClickListener {

	private Button btnSelectEnPng = null;
	private TextView txtEnPng = null;
	private Button btnToGet = null;
	
	private String enpngFile = null;
	private final String ENPNGFILEKEY = "ENPNGFILE";
	private final int TOGET = 3; 
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_readfile);
		btnSelectEnPng = (Button) findViewById(R.id.btn_get_enpng);
		btnToGet = (Button) findViewById(R.id.btn_toget);
		txtEnPng = (TextView) findViewById(R.id.txt_enpng);
		btnSelectEnPng.setOnClickListener(this);
		btnToGet.setOnClickListener(this);
	}

	Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
			case TOGET:
				Bundle data = msg.getData();
				String enpngStr = data.getString(ENPNGFILEKEY);
				new PngHandler(PngHandler.SAVE2ALPHALOW).getFileFromPng(enpngStr);
				break;
			}
		}
	};
	
	@Override
	public void onClick(View arg0) {
		Intent intent = new Intent();
		switch (arg0.getId()) {
		case R.id.btn_get_enpng:
			intent.putExtra(Config.explorer_title, getString(R.string.get_file_title));
			intent.setDataAndType(Uri.fromFile(new File(Config.top_dir)), "*/*");
			intent.setClass(this, ExDialog.class);
			startActivityForResult(intent, Config.REQUEST_ENPNG);
			break;
		case R.id.btn_toget:
			if ((enpngFile==null)) {
				Toast.makeText(getApplicationContext(), "输入文件为空", Toast.LENGTH_SHORT).show();
				break;
			}
			if (!enpngFile.endsWith(".png")) {
				Toast.makeText(getApplicationContext(), "输入文件为空", Toast.LENGTH_SHORT).show();
				break;
			}
			Message msg = new Message();
			msg.what = TOGET;
			Bundle data = new Bundle();
			data.putString(ENPNGFILEKEY, enpngFile);
			msg.setData(data);
			handler.sendMessage(msg);
			break;
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
		super.onActivityResult(requestCode, resultCode, intent);
		if (RESULT_OK==resultCode) {
			Uri uri = intent.getData();
			if (Config.REQUEST_ENPNG==requestCode) {
				enpngFile = uri.getPath();
				txtEnPng.setText("enpng select: " + enpngFile);
			}
		}
	}

}

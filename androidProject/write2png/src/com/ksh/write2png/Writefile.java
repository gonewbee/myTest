package com.ksh.write2png;

import java.io.File;

import com.ksh.pngops.PngHandler;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class Writefile extends Activity implements View.OnClickListener {

	private Button btn_browse_file;
	private Button btn_browse_png;
	private Button btn_ok;
	private TextView txt_src_file;
	private TextView txt_load_png;
	
	private String src_file = null;
	private String png_file = null;
	private Handler dialogHandler = null;
	private ProgressDialog dialog = null;
	private final String SRCFILEKEY = "SRCFILE";
	private final String PNGFILEKEY = "PNGFILE";
	private final int STARTWRITE = 1; 
	private final int SHOWDIALOG = 2;
	private final int CANCELDIALOG = 3;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_writefile);
		btn_browse_file = (Button) findViewById(R.id.btn_browsw_file);
		btn_browse_png = (Button) findViewById(R.id.btn_browse_png);
		btn_ok = (Button) findViewById(R.id.btn_write_ok);
		txt_src_file = (TextView) findViewById(R.id.txt_src_file);
		txt_load_png = (TextView) findViewById(R.id.txt_load_png);
		btn_browse_file.setOnClickListener(this);
		btn_browse_png.setOnClickListener(this);
		btn_ok.setOnClickListener(this);
		
		HandlerThread dialogThread = new HandlerThread("dialogThread");
		dialogThread.start();
		dialogHandler = new Handler(dialogThread.getLooper()) {
			public void handleMessage(Message msg) {
				super.handleMessage(msg);
				switch (msg.what) {
				case SHOWDIALOG:
					dialog = ProgressDialog.show(Writefile.this, "提示", "正在保存文件", true);
					break;
				case CANCELDIALOG:
					if (dialog!=null)
						dialog.cancel();
					break;
				}
			}
		};
	}
	
	Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
			case STARTWRITE:
				Bundle data = msg.getData();
				String srcFile = data.getString(SRCFILEKEY);
				String pngFile = data.getString(PNGFILEKEY);
				dialogHandler.removeMessages(SHOWDIALOG);
				dialogHandler.sendEmptyMessage(SHOWDIALOG);
				boolean ret = new PngHandler(PngHandler.SAVE2ALPHALOW).savefile2png(srcFile, pngFile);
				dialogHandler.removeMessages(CANCELDIALOG);
				dialogHandler.sendEmptyMessage(CANCELDIALOG);
				String str;
				if (ret)
					str = "隐写成功";
				else
					str = "隐写失败";
				Toast toast = Toast.makeText(getApplicationContext(), str, Toast.LENGTH_SHORT);
				toast.setGravity(Gravity.CENTER, 0, 0);
				toast.show();
				break;
			}
		}
	};
	
	@Override
	public void onClick(View arg0) {
		Intent intent = new Intent();
		switch (arg0.getId()) {
		case R.id.btn_browsw_file:
			intent.putExtra(Config.explorer_title, getString(R.string.get_file_title));
			intent.setDataAndType(Uri.fromFile(new File(Config.top_dir)), "*/*");
			intent.setClass(this, ExDialog.class);
			startActivityForResult(intent, Config.REQUEST_SRCFILE);
			break;
		case R.id.btn_browse_png:
			intent.putExtra(Config.explorer_title, getString(R.string.browse_png));
			intent.setDataAndType(Uri.fromFile(new File(Config.top_dir)), "*/*");
			intent.setClass(this, ExDialog.class);
			startActivityForResult(intent, Config.REQUEST_PNGFILE);
			break;
		case R.id.btn_write_ok:
			if ((src_file==null) || (png_file==null)) {
				Toast.makeText(getApplicationContext(), "输入文件为空", Toast.LENGTH_SHORT).show();
				break;
			}
			if (!png_file.endsWith(".png")) {
				Toast.makeText(getApplicationContext(), "选择的不是png文件", Toast.LENGTH_SHORT).show();
				break;
			}
			Message msg = new Message();
			msg.what = STARTWRITE;
			Bundle data = new Bundle();
			data.putString(SRCFILEKEY, src_file);
			data.putString(PNGFILEKEY, png_file);
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
			if (Config.REQUEST_SRCFILE==requestCode) {
				src_file = uri.getPath();
				txt_src_file.setText("src select: " + src_file);
			} else if (Config.REQUEST_PNGFILE==requestCode) {
				png_file = uri.getPath();
				txt_load_png.setText("png select: " + png_file);
			}
		}
	}

}

package com.ksh.update;

import java.io.File;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.view.Gravity;
import android.widget.Toast;

public class CheckUpdateTask extends AsyncTask<String, Void, File>{

	private Context mContext;
	public CheckUpdateTask(Context cxt) {
		mContext = cxt;
	}
	protected File doInBackground(String... params) {
		String filePath = mContext.getFilesDir() + File.separator + "write2png.apk";
		System.out.println("UpdateTask: " + filePath);
		File file = new File(filePath);
		if (!file.exists()) {
			System.out.println("UpdateTask: file is not exist");
			return null;
		}
		return file;
	}

	protected void onPostExecute(final File result) {
		super.onPostExecute(result);
		if (null==result) {
			Toast toast = Toast.makeText(mContext, "文件不存在", Toast.LENGTH_SHORT);
			toast.setGravity(Gravity.CENTER, 0, 0);
			toast.show();
			return;
		}
		
		Dialog dialog = new AlertDialog.Builder(mContext)
		.setTitle("软件更新")
		.setMessage("是否更新")
		.setPositiveButton("确定", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				System.out.println("UpdateTask: ok");
				update(result);
			}
		})
		.setNegativeButton("取消", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				System.out.println("UpdateTask: cancle");
				dialog.cancel();
			}
		})
		.create();
		dialog.show();
	}
	
	private void update(File file) {
		Intent intent = new Intent();
		intent.setDataAndType(Uri.fromFile(file), "application/vnd.android.package-archive");
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		mContext.startActivity(intent);
	}

}

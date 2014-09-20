package com.ksh.update;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Gravity;
import android.widget.Toast;

public class CheckUpdateTask extends AsyncTask<String, Void, Bundle>{

	private Context mContext;
	private final String apkName = "write2png.apk";
	private final String jsonName = "apkinfo.json";
	private final String DATABRET = "bundleIsSuccess";
	private final String DATASDES = "bundleDesc";
	private final String DATAFILE = "bundleFile";
	private final String JAPKCODE = "versionCode";
	private final String JJSONURL = "jsonURL";
	private final String JAPKURL = "apkURL";
	private final String JMD5 = "md5";
	private String md5FromJson = "";
	
	public CheckUpdateTask(Context cxt) {
		mContext = cxt;
	}
	protected Bundle doInBackground(String... params) {
		boolean ret;
		Bundle bData = new Bundle();
		ret = downloadJson();
		if (!ret) {
			bData.putBoolean(DATABRET, false);
			bData.putString(DATASDES, "下载json文件失败");
			return bData;
		}
		String apkUrl = getDownloadApkUrl();
		if (null==apkUrl) {
			bData.putBoolean(DATABRET, false);
			bData.putString(DATASDES, "不需要升级");
			return bData;
		}
		ret = downloadFile(apkUrl, apkName, mContext.MODE_WORLD_READABLE);
		if (!ret) {
			bData.putBoolean(DATABRET, false);
			bData.putString(DATASDES, "下载apk失败");
			return bData;
		}
		String filePath = mContext.getFilesDir() + File.separator + apkName;
		System.out.println("CheckUpdateTask: " + filePath);
		String md5l = getFileMd5(filePath);
		System.out.println("CheckUpdateTask download file md5: " + md5l);
		if (md5l.equals(md5FromJson)) {
			bData.putBoolean(DATABRET, true);
			bData.putString(DATAFILE, filePath);
		} else {
			bData.putBoolean(DATABRET, false);
			bData.putString(DATASDES, "下载apk有误");
		}
		return bData;
	}

	protected void onPostExecute(final Bundle bData) {
		super.onPostExecute(bData);
		boolean ret = bData.getBoolean(DATABRET);
		if (!ret) {
			String retDes = bData.getString(DATASDES);
			System.out.println("CheckUpdateTask onPostExecute: " + retDes);
			Toast toast = Toast.makeText(mContext, retDes, Toast.LENGTH_SHORT);
			toast.setGravity(Gravity.CENTER, 0, 0);
			toast.show();
			return;
		}
		
		Dialog dialog = new AlertDialog.Builder(mContext)
		.setTitle("文件隐写器更新")
		.setMessage("是否更新")
		.setPositiveButton("确定", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				System.out.println("UpdateTask: ok");
				File file = new File(bData.getString(DATAFILE));
				update(file);
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
	
	/**
	 * 根据json文件判断是否升级
	 * @return apk若需要升级返回升级url，否则返回null
	 */
	private String getDownloadApkUrl() {
		try {
			FileInputStream fis = mContext.openFileInput(jsonName);
			byte[] data = new byte[fis.available()];
			fis.read(data);
			fis.close();
			String jsonStr = new String(data);
			JSONTokener jsonParser = new JSONTokener(jsonStr);
			JSONObject apkinfo = (JSONObject) jsonParser.nextValue();
			int apkVersion = apkinfo.getInt(JAPKCODE);
			System.out.println("CheckUpdateTask apkVersion is " + apkVersion);
			if (checkVersionCode(apkVersion)) {
				String apkUrl = apkinfo.getString(JAPKURL);
				md5FromJson = apkinfo.getString(JMD5);
				return apkUrl;
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	private boolean checkVersionCode(int jsonApkVersion) {
		PackageManager manager = mContext.getPackageManager();
		try {
			PackageInfo info = manager.getPackageInfo(mContext.getPackageName(), 0);
			int mversionCode = info.versionCode;
			if (jsonApkVersion>mversionCode)
				return true;
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	private boolean downloadJson() {
		String jsonHost = getJsonHost();
		return downloadFile(jsonHost, jsonName, mContext.MODE_PRIVATE);
	}
	
	private String getJsonHost() {
		File jsonFile = new File(mContext.getFilesDir() + File.separator + jsonName);
		if (!jsonFile.exists()) {
			try {
				InputStream is = mContext.getResources().getAssets().open(jsonName);
				FileOutputStream fos = mContext.openFileOutput(jsonName, mContext.MODE_PRIVATE);
				byte[] buf = new byte[is.available()];
				is.read(buf);
				fos.write(buf);
				is.close();
				fos.close();
			} catch (IOException e) {
				e.printStackTrace();
				return null;
			}
		}
		try {
			FileInputStream fis = mContext.openFileInput(jsonName);
			byte[] data = new byte[fis.available()];
			fis.read(data);
			fis.close();
			String jsonStr = new String(data);
			System.out.println("CheckUpdateTask: input json is " + jsonStr);
			JSONTokener jsonParser = new JSONTokener(jsonStr);
			JSONObject apkinfo = (JSONObject) jsonParser.nextValue();
			String jsonHost = apkinfo.getString(JJSONURL);
			System.out.println("CheckUpdateTask: jsonHost is " + jsonHost);
			return jsonHost;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	private boolean downloadApk() {
		try {
			URL url = new URL("http://192.168.11.85/files/write2png.apk");
			HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
			urlConnection.setConnectTimeout(5*1000);
			InputStream in = new BufferedInputStream(urlConnection.getInputStream());
			byte[] buf = new byte[1024];
			int len = 0;
			FileOutputStream fos = mContext.openFileOutput(apkName, mContext.MODE_WORLD_READABLE);
			while ((len=in.read(buf))>0) {
				fos.write(buf, 0, len);
			}
			in.close();
			fos.close();
			urlConnection.disconnect();
			return true;
		} catch (MalformedURLException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	/**
	 * 下载文件
	 * @param urlStr 文件网络地址
	 * @param fileName 保存文件名
	 * @return true：下载成功	false：下载失败
	 */
	private boolean downloadFile(String urlStr, String fileName, int mode) {
		try {
			URL url = new URL(urlStr);
			HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
			urlConnection.setConnectTimeout(5*1000);
			InputStream in = new BufferedInputStream(urlConnection.getInputStream());
			byte[] buf = new byte[1024];
			int len = 0;
			FileOutputStream fos = mContext.openFileOutput(fileName, mode);
			while ((len=in.read(buf))>0) {
				fos.write(buf, 0, len);
			}
			in.close();
			fos.close();
			urlConnection.disconnect();
			return true;
		} catch (MalformedURLException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	private void update(File file) {
		Intent intent = new Intent();
		intent.setDataAndType(Uri.fromFile(file), "application/vnd.android.package-archive");
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		mContext.startActivity(intent);
	}

	private String getFileMd5(String fileName) {
		String md5s = "";
		try {
			FileInputStream fis = new FileInputStream(fileName);
			int len = fis.available();
			byte[] buf = new byte[len];
			int readLen = fis.read(buf);
			MessageDigest md5 = MessageDigest.getInstance("MD5");
			md5.update(buf, 0, readLen);
			byte[] ret = md5.digest();
			int i = 0;
			for (i=0; i<ret.length; i++) {
				md5s += String.format("%02x", ret[i]);
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}
		return md5s;
	}
}

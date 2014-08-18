/**
 * 利用png无损压缩的特性，将jar文件隐写到png的alpha通道。再将dex文件通过DexClassLoader加载
 */
package com.example.usedex;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.Random;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import com.dex.test.IDextest;
import com.example.jni.Getso;

import dalvik.system.DexClassLoader;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
import android.os.Build;

@SuppressLint("ValidFragment")
public class UseDex extends Activity {

	public static final String TAG = "UseDex";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_use_dex);

		if (savedInstanceState == null) {
			getFragmentManager().beginTransaction()
					.add(R.id.container, new PlaceholderFragment()).commit();
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.use_dex, menu);
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

	/**
	 * A placeholder fragment containing a simple view.
	 */
	@SuppressLint("ValidFragment")
	public class PlaceholderFragment extends Fragment implements OnClickListener{
		public Button btn_dex;
		private IDextest lib = null;
		
		public PlaceholderFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_use_dex,
					container, false);
			return rootView;
		}

		@Override
		public void onActivityCreated(Bundle savedInstanceState) {
			super.onActivityCreated(savedInstanceState);
			btn_dex = (Button)findViewById(R.id.btn_dex);
			btn_dex.setOnClickListener(this);
			getFileFromPng();
			String dexPath = getFilesDir().toString() + File.separator + "test.jar";
//			String dexPath = Environment.getExternalStorageDirectory().toString() + File.separator + "test.jar";
//			String dexOutputDirs = Environment.getExternalStorageDirectory().toString();
//			String dexOutputDirs = getFilesDir().toString();
			String dexOutputDirs = getDir("dex", 0).getAbsolutePath();
			System.out.println("dexPath: " + dexPath);
			System.out.println("outputDir: " + dexOutputDirs);
			DexClassLoader cl = new DexClassLoader(dexPath,dexOutputDirs,null,getClassLoader());
			//使用DexClassLoader接口dex文件会保存在系统中，用完后删除dex文件，下次在用重新加载
			new File(dexPath).delete();
			new File(dexOutputDirs+"/test.dex").delete();
			Class libProviderClazz = null;
			try {
				libProviderClazz = cl.loadClass("com.dex.test.Dextest");
				lib  = (IDextest)libProviderClazz.newInstance();
			} catch (Exception e) {
				e.printStackTrace();
				btn_dex.setClickable(false);
			}
			testBuf();
		}

		@Override
		public void onClick(View v) {
			switch(v.getId()) {
			case R.id.btn_dex:
				Toast.makeText(getApplicationContext(), lib.getDexString(), Toast.LENGTH_SHORT).show();
				break;
			}
		}
		
		/**
		 * 从png的alpha通道中取出文件，前四个字节是文件长度
		 * @return
		 */
		private boolean getFileFromPng() {
			InputStream in = getResources().openRawResource(R.drawable.cvout);
			Bitmap bmp = BitmapFactory.decodeStream(in);
			try {
				in.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			int fileLen = Color.alpha(bmp.getPixel(0, 0))<<24;
			fileLen += Color.alpha(bmp.getPixel(1, 0))<<16;
			fileLen += Color.alpha(bmp.getPixel(2, 0))<<8;
			fileLen += Color.alpha(bmp.getPixel(3, 0));
			System.out.println("fileLen: " + fileLen);
			int width = bmp.getWidth();
			int heigth = bmp.getHeight();
			if ((fileLen+4)>width*heigth) {
				Log.d(TAG, "file not in png");
				return false;
			}
			int readFromPngLen=0;
			int i=0,j=0;
			int needReadLen = fileLen+4;
			byte[] fileContext = new byte[needReadLen];
			for (j=0; j<heigth; j++) {
				for (i=0; i<width; i++) {
					fileContext[readFromPngLen++] = (byte) Color.alpha(bmp.getPixel(i, j));
					if (readFromPngLen>=needReadLen)
						break;
				}
				if (readFromPngLen>=needReadLen)
					break;
			}
			return decodeAesCBC(fileContext, "Sixteen byte key".getBytes());
		}
		
		private boolean decodeAesCBC(byte[] data, byte[] passwd) {
				SecretKeySpec key = new SecretKeySpec(passwd, "AES");
				Cipher cipher;
				try {
					cipher = Cipher.getInstance("AES/CBC/NoPadding");
					byte[] ivByte = new byte[16];
					new Random().nextBytes(ivByte);
					IvParameterSpec iv = new IvParameterSpec(ivByte);
					cipher.init(2, key, iv);
					byte[] byteContent = new byte[data.length-4];
					System.arraycopy(data, 4, byteContent, 0, data.length-4);
					byte[] result;
					result = cipher.doFinal(byteContent);
					int fileLen = ((result[16]&0xff)<<24)+((result[17]&0xff)<<16)+((result[18]&0xff)<<8)+(result[19]&0xff);
					FileOutputStream out;
					out = openFileOutput("test.jar", MODE_PRIVATE);
					if ((fileLen+20)>result.length)
						out.write(result, 20, result.length-20);
					else
						out.write(result, 20, fileLen);
					out.close();
					return true;
				} catch (NoSuchAlgorithmException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (NoSuchPaddingException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (InvalidKeyException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (InvalidAlgorithmParameterException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IllegalBlockSizeException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (BadPaddingException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (FileNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				return false;
		}
		
		private void testBuf() {
			Getso getso = new Getso();
			Log.d(TAG, getso.getVersion());
			byte[] in = {0x1, 0x2, 0x3, 0x4};
			byte[] key = {0x10, 0x20, 0x30, 0x40};
			byte[] out = new byte[in.length];
			getso.getBuf(in, out, key);
			System.out.printf("ret:%x %x %x %x\n", out[0], out[1], out[2], out[3]);
		}
		
	}

}

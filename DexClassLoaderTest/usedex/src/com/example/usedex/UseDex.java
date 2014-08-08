package com.example.usedex;

import java.io.File;

import com.dex.test.IDextest;

import dalvik.system.DexClassLoader;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.os.Bundle;
import android.os.Environment;
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
			String dexPath = Environment.getExternalStorageDirectory().toString() + File.separator + "test.jar";
//			String dexOutputDirs = Environment.getExternalStorageDirectory().toString();
			String dexOutputDirs = getFilesDir().toString();
			System.out.println("dexPath: " + dexPath);
			DexClassLoader cl = new DexClassLoader(dexPath,dexOutputDirs,null,getClassLoader());
			Class libProviderClazz = null;
			try {
				libProviderClazz = cl.loadClass("com.dex.test.Dextest");
				lib  = (IDextest)libProviderClazz.newInstance();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		@Override
		public void onClick(View v) {
			switch(v.getId()) {
			case R.id.btn_dex:
				Toast.makeText(getApplicationContext(), lib.getDexString(), Toast.LENGTH_SHORT).show();
				break;
			}
		}
		
	}

}

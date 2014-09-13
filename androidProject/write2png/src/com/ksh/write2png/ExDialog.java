package com.ksh.write2png;

import android.os.Bundle;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.ImageView;
import android.widget.Button;
import android.widget.BaseAdapter;
import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;
import android.content.Context;
import android.util.Log;
import android.view.View.OnClickListener;
import android.content.Intent;
import android.graphics.Point;
import android.net.Uri;
import android.app.ListActivity;
import android.view.WindowManager;
import android.view.Display;
import android.view.WindowManager.LayoutParams;

import java.util.List;
import java.util.Map;
import java.util.ArrayList;
import java.util.HashMap;
import java.io.File;

public class ExDialog extends ListActivity {
	private List<Map<String, Object>> mData;
	private String mDir = Config.top_dir;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		Intent intent = this.getIntent();
		Bundle bl = intent.getExtras();
		String title = bl.getString("explorer_title");
		Uri uri = intent.getData();
		mDir = uri.getPath();

		setTitle(title);
		mData = getData();
		MyAdapter adapter = new MyAdapter(this);
		setListAdapter(adapter);

		WindowManager m = getWindowManager();
		Display d = m.getDefaultDisplay();
		Point size = new Point();
		LayoutParams p = getWindow().getAttributes();
		d.getSize(size);
		p.height = (int) (size.y * 0.8);
		p.width = (int) (size.x * 0.95);
		getWindow().setAttributes(p);
	}

	private List<Map<String, Object>> getData() {
		List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
		Map<String, Object> map = null;
		File f = new File(mDir);
		File[] files = f.listFiles();

		if (!mDir.equals(Config.top_dir)) {
			map = new HashMap<String, Object>();
			map.put("title", "Back to ../");
			map.put("info", f.getParent());
			map.put("img", R.drawable.ex_folder);
			list.add(map);
		}
		if (files != null) {
			for (int i = 0; i < files.length; i++) {
				map = new HashMap<String, Object>();
				map.put("title", files[i].getName());
				map.put("info", files[i].getPath());
				if (files[i].isDirectory())
					map.put("img", R.drawable.ex_folder);
				else
					map.put("img", R.drawable.ex_doc);
				list.add(map);
			}
		}
		return list;
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		Log.d("MyListView4-click", (String) mData.get(position).get("info"));
		if ((Integer) mData.get(position).get("img") == R.drawable.ex_folder) {
			mDir = (String) mData.get(position).get("info");
			mData = getData();
			MyAdapter adapter = new MyAdapter(this);
			setListAdapter(adapter);
		} else {
			finishWithResult((String) mData.get(position).get("info"));
		}
	}

	public final class ViewHolder {
		public ImageView img;
		public TextView title;
		public TextView info;
	}

	public class MyAdapter extends BaseAdapter {
		private LayoutInflater mInflater;

		public MyAdapter(Context context) {
			this.mInflater = LayoutInflater.from(context);
		}

		public int getCount() {
			return mData.size();
		}

		public Object getItem(int arg0) {
			return null;
		}

		public long getItemId(int arg0) {
			return 0;
		}

		public View getView(int position, View convertView, ViewGroup parent) {
			ViewHolder holder = null;
			if (convertView == null) {
				holder = new ViewHolder();
				convertView = mInflater.inflate(R.layout.listview, null);
				holder.img = (ImageView) convertView.findViewById(R.id.img);
				holder.title = (TextView) convertView.findViewById(R.id.title);
				holder.info = (TextView) convertView.findViewById(R.id.info);
				convertView.setTag(holder);
			} else {
				holder = (ViewHolder) convertView.getTag();
			}

			holder.img.setBackgroundResource((Integer) mData.get(position).get(
					"img"));
			holder.title.setText((String) mData.get(position).get("title"));
			holder.info.setText((String) mData.get(position).get("info"));
			return convertView;
		}
	}

	private void finishWithResult(String path) {
		Bundle conData = new Bundle();
		conData.putString("results", "selected file");
		Intent intent = new Intent();
		intent.putExtras(conData);
		Uri startDir = Uri.fromFile(new File(path));
		System.out.println("in result: " + path);
		intent.setDataAndType(startDir,
				"vnd.android.cursor.dir/lysesoft.andexplorer.file");
		setResult(RESULT_OK, intent);
		finish();
	}
};


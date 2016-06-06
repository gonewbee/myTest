package com.example.zsy.glestutorial;

import android.content.Intent;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.example.zsy.util.ListItem;

import java.util.List;

/**
 * Created by zsy on 16-4-21.
 */
public class MyAdapter extends RecyclerView.Adapter<MyAdapter.ViewHolder> {
    private List<ListItem> mDataset;

    public class ViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        public TextView mTextView;

        public ViewHolder(View v) {
            super(v);
            v.setOnClickListener(this);
            mTextView = (TextView) v.findViewById(R.id.my_item_text);
        }

        @Override
        public void onClick(View v) {
//            Log.i("MyAdapter", "onClick!!");
            Toast.makeText(v.getContext(), "data:" + mTextView.getText(), Toast.LENGTH_SHORT).show();
            Intent intent = new Intent(v.getContext(), mDataset.get(getAdapterPosition()).cls);
            v.getContext().startActivity(intent);
        }
    }

    public MyAdapter(List<ListItem> myDateset) {
        mDataset = myDateset;
    }


    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(
                parent.getContext()).inflate(R.layout.my_item, parent,
                false);

        ViewHolder vh = new ViewHolder(v);
        return vh;
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        holder.mTextView.setText(mDataset.get(position).name);
    }

    @Override
    public int getItemCount() {
        return mDataset.size();
    }

}

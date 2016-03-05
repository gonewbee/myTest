package com.example.zsy.hellogles;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.Configuration;
import android.opengl.GLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity {

    private GLSurfaceView mGLSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_main);
        mGLSurfaceView = new GLSurfaceView(this);
        if (detectOpenGLES20()) {
            mGLSurfaceView.setEGLContextClientVersion(2);
            mGLSurfaceView.setRenderer(new MyRender());
        } else {
            Log.e("HelloGLES", "OpenGL ES 2.0 not supported on device. Exiting...");
            finish();
        }
        setContentView(mGLSurfaceView);
    }


    private boolean detectOpenGLES20() {
        ActivityManager am =
                (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        // 打印OpenGL ES版本信息
        Log.i("HelloGLES", String.format("GLESVersion:%x\n", info.reqGlEsVersion));
        return (info.reqGlEsVersion >= 0x20000);
    }
}

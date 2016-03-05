package com.example.zsy.hellogles;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by zsy on 16-3-4.
 */
public class MyRender implements GLSurfaceView.Renderer {
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.i("HelloGLES", "onSurfaceCreated");
        GLES2JNILib.onNDKSurfaceCreated();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.i("HelloGLES", "onSurfaceChanged width:" + width + " height:" + height);
        GLES2JNILib.onNDKSurfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        Log.i("HelloGLES", "onDrawFrame");
        GLES2JNILib.onNDKDrawFrame();
    }
}

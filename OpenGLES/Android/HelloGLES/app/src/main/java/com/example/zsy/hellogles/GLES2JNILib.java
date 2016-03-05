package com.example.zsy.hellogles;

/**
 * Created by zsy on 16-3-5.
 */
public class GLES2JNILib {

    static {
        System.loadLibrary("gles2jni");
    }

    @SuppressWarnings("JniMissingFunction")
    public static native void onNDKSurfaceCreated();

    @SuppressWarnings("JniMissingFunction")
    public static native void onNDKSurfaceChanged(int width, int height);

    @SuppressWarnings("JniMissingFunction")
    public static native void onNDKDrawFrame();
}

#include <jni.h>
#include "gles2_triangle.h"

JNIEXPORT void JNICALL Java_com_example_zsy_hellogles_GLES2JNILib_onNDKSurfaceCreated
  (JNIEnv *env, jclass obj) {
    triangle_init();
}

JNIEXPORT void JNICALL Java_com_example_zsy_hellogles_GLES2JNILib_onNDKSurfaceChanged
  (JNIEnv *env, jclass obj, jint width, jint height) {
    resize(width, height);
}

JNIEXPORT void JNICALL Java_com_example_zsy_hellogles_GLES2JNILib_onNDKDrawFrame
  (JNIEnv *env, jclass obj) {
    triangle_draw();
}


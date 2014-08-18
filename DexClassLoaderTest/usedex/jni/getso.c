#include <jni.h>
#include <android/log.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG, "libgetso", __VA_ARGS__)

JNIEXPORT jstring JNICALL Java_com_example_jni_Getso_getVersion
  (JNIEnv *env, jobject thiz) {
    return (*env)->NewStringUTF(env, "libGetso version 0.1");
}


JNIEXPORT jboolean JNICALL Java_com_example_jni_Getso_getBuf
  (JNIEnv *env, jobject thiz, jbyteArray bIn, jbyteArray bOut, jbyteArray bKey) {
    jbyte *in = (*env)->GetByteArrayElements(env, bIn, NULL);
    jbyte *key = (*env)->GetByteArrayElements(env, bKey, NULL);
    int len = sizeof(in);
    LOGI("sizeof of input is:%d\n", len);
    uint8_t *ret = (uint8_t *)malloc(len);
    int i = 0;
    for (i=0; i<len; i++) 
        ret[i] = (in[i]&0xff) + (key[i]&0xff);
    (*env)->ReleaseByteArrayElements(env, bIn, in, 0);
    (*env)->ReleaseByteArrayElements(env, bKey, key, 0);
    (*env)->SetByteArrayRegion(env, bOut, 0, len, (const jbyte *)ret);
    free(ret);
    return 1;
}

#ifdef __cplusplus
}
#endif

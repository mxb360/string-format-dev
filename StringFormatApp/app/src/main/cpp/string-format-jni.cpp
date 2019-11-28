#include <jni.h>

#include "string-format.h"

extern "C" JNIEXPORT
jint JNICALL Java_com_example_stringformatapp_StringFormat_open(JNIEnv *env, jobject thiz) {
    return string_format_open();
}

extern "C" JNIEXPORT
jstring JNICALL Java_com_example_stringformatapp_StringFormat_cmd(JNIEnv *env, jobject thiz,
                                                                  jstring string, jint cmd, jint sf) {
    char dest[128];
    const char *src = env->GetStringUTFChars(string, 0);
    char *res = NULL;

    if (string_format_cmd(dest, src, 128, sf, cmd))
        res = dest;

    env->ReleaseStringUTFChars(string, src);
    return env->NewStringUTF(res);

}

extern "C" JNIEXPORT
jint JNICALL Java_com_example_stringformatapp_StringFormat_close(JNIEnv *env, jobject thiz,
                                                                 jint sf) {
    return string_format_close(sf);
}

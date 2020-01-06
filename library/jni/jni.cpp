//
// Created by ben622 on 2020/1/6.
//
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include "qrencode.hpp"


jint nativeQREncode(JNIEnv *env, jclass c, jstring jqrsource,jint prescaler,jstring joutput,jint r,jint g,jint b) {
    char *qrsource = const_cast<char *>(env->GetStringUTFChars(jqrsource, false));
    char *output = const_cast<char *>(env->GetStringUTFChars(joutput, false));
    int code = qrencode::encode(qrsource, prescaler, output, r, g, b);
    env->ReleaseStringChars(jqrsource, reinterpret_cast<const jchar *>(qrsource));
    env->ReleaseStringChars(joutput, reinterpret_cast<const jchar *>(output));
    return code;
}
static const char *JavaQREncodeClassName = "com/ben/android/qrencode/library/QREncode";
static JNINativeMethod QREncodeMethods[] = {
        {"nativeQREncode", "(Ljava/lang/String;ILjava/lang/String;III)I", (void *) nativeQREncode}
};

int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods,
                                 int methodsCount) {
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, methods, methodsCount) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = 0;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    if (!registerNativeMethods(env, JavaQREncodeClassName, QREncodeMethods,
                               sizeof(QREncodeMethods) /
                               sizeof(QREncodeMethods[0]))) {
        return JNI_FALSE;
    }

    return JNI_VERSION_1_6;
}
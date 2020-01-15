//
// Created by ben622 on 2020/1/15.
//

#ifndef LIBQRENCODE_ANDROID_DECORATE_H
#define LIBQRENCODE_ANDROID_DECORATE_H
#include <jni.h>
#include <cstring>
#include <unistd.h>
#include "log.h"
#include "util.h"
#include <android/bitmap.h>
namespace qrencode{
    namespace decorate{
        void insertLogo(JNIEnv *env,AndroidBitmapInfo *pixelInfo,void *pixels, jobject logo);
    }
}
#endif //LIBQRENCODE_ANDROID_DECORATE_H

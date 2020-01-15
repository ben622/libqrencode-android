//
// Created by ben622 on 2020/1/15.
//

#ifndef LIBQRENCODE_ANDROID_LOG_H
#define LIBQRENCODE_ANDROID_LOG_H
#include <jni.h>
#include <android/log.h>
#define LOGS_ENABLED true
#define PRINT_TAG "qrencode"
#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_VERBOSE,PRINT_TAG,FORMAT,__VA_ARGS__)
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,PRINT_TAG,FORMAT,__VA_ARGS__)
#endif //LIBQRENCODE_ANDROID_LOG_H

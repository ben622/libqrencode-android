//
// Created by ben622 on 2020/1/2.
//
#ifndef TELEGRAM_QRENCODE_H
#define TELEGRAM_QRENCODE_H

#include <jni.h>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include "./libqrencode/include/qrencode.h"
#include <android/bitmap.h>
#include "log.h"
#include "util.h"
#include "decorate.h"
// default colors.
#define DEFAULT_QR_COLOR            0xFF000000
#define PIXEL_COLOR_R                0
#define PIXEL_COLOR_G                0
#define PIXEL_COLOR_B                0
//不压缩
#define BI_RGB            0L

// BMP defines
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef signed long LONG;


#pragma pack(push, 2)

typedef struct {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

namespace qrencode{
    jint encode(const char *qrsource, unsigned int prescaler, const char *output);

    jint encode(const char *qrsource, unsigned int prescaler, const char *output, unsigned int r,
                unsigned int g, unsigned int b);

    jobject encode(JNIEnv *env,const char *qrsource, unsigned int prescaler, unsigned int backColor, unsigned int qrColor,jobject logo);

    jobject encode(JNIEnv *env,const char *qrsource, unsigned int prescaler, unsigned int backColor, jobject overly,jobject logo);

    jobject createBitmap(JNIEnv *env, uint32_t width, uint32_t height);

}


#endif //TELEGRAM_QRENCODE_H

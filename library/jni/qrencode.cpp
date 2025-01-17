//
// Created by ben622 on 2020/1/2.
//

#include "qrencode.hpp"

jint qrencode::encode(const char *qrsource, unsigned int prescaler, const char *output) {
    return encode(qrsource, prescaler, output, 0, 0, 0);
}

jint
qrencode::encode(const char *qrsource, unsigned int prescaler, const char *output, unsigned int r,
                 unsigned int g, unsigned int b) {
    unsigned int unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
    unsigned char *pRGBData, *pSourceData, *pDestData;
    QRcode *pQRC;
    FILE *f;

    if (pQRC = QRcode_encodeString(qrsource, 0, QR_ECLEVEL_H, QR_MODE_8, 1)) {
        //根据输入大小自动放大缩放二维码
        prescaler = prescaler / pQRC->width;
        unWidth = pQRC->width;
        unWidthAdjusted = unWidth * prescaler * 3;
        if (unWidthAdjusted % 4)
            unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
        unDataBytes = unWidthAdjusted * unWidth * prescaler;

        // Allocate pixels buffer
        if (!(pRGBData = (unsigned char *) malloc(unDataBytes))) {
            if (LOGS_ENABLED) LOGE("%s", "Out of memory");
            return -1;
        }
        // Preset to white
        memset(pRGBData, 0xff, unDataBytes);

        // Prepare bmp headers
        BITMAPFILEHEADER kFileHeader;
        kFileHeader.bfType = 0x4d42;  // "BM"
        kFileHeader.bfSize = sizeof(BITMAPFILEHEADER) +
                             sizeof(BITMAPINFOHEADER) +
                             unDataBytes;
        //保留位，必须为0
        kFileHeader.bfReserved1 = 0;
        //保留位，必须位0
        kFileHeader.bfReserved2 = 0;
        //从文件头开始到实际的图像数据的字节偏移量
        kFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) +
                                sizeof(BITMAPINFOHEADER);

        BITMAPINFOHEADER kInfoHeader;
        kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
        kInfoHeader.biWidth = unWidth * prescaler;
        //图像是正向
        kInfoHeader.biHeight = -((int) unWidth * prescaler);
        kInfoHeader.biPlanes = 1;
        //24位
        kInfoHeader.biBitCount = 24;
        //不压缩
        kInfoHeader.biCompression = BI_RGB;
        kInfoHeader.biSizeImage = 0;
        kInfoHeader.biXPelsPerMeter = 0;
        kInfoHeader.biYPelsPerMeter = 0;
        kInfoHeader.biClrUsed = 0;
        kInfoHeader.biClrImportant = 0;


        // Convert QrCode bits to bmp pixels
        pSourceData = pQRC->data;
        for (y = 0; y < unWidth; y++) {
            pDestData = pRGBData + unWidthAdjusted * y * prescaler;
            for (x = 0; x < unWidth; x++) {
                //如果这个点为1，则该点存在数据
                if (*pSourceData & 1) {
                    //写rgb
                    for (l = 0; l < prescaler; l++) {
                        for (n = 0; n < prescaler; n++) {
                            *(pDestData + n * 3 + unWidthAdjusted * l) = (b > 0 ? b
                                                                                : PIXEL_COLOR_B);
                            *(pDestData + 1 + n * 3 + unWidthAdjusted * l) = (g > 0 ? g
                                                                                    : PIXEL_COLOR_G);
                            *(pDestData + 2 + n * 3 + unWidthAdjusted * l) = (r > 0 ? r
                                                                                    : PIXEL_COLOR_R);
                        }
                    }
                }
                pDestData += 3 * prescaler;
                pSourceData++;
            }
        }

        f = fopen(output, "wb");
        if (f != NULL) {
            fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
            fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
            fwrite(pRGBData, sizeof(unsigned char), unDataBytes, f);
            fclose(f);
        } else {
            if (LOGS_ENABLED) LOGE("%s", "Unable to open file");
            return -1;
        }

        // Free data
        free(pRGBData);
        QRcode_free(pQRC);
        if (LOGS_ENABLED) LOGI("qrencode successful,outputfile:%s", output);
    } else {
        if (LOGS_ENABLED) LOGE("%s", "NULL returned");
        return -1;
    }
    return 0;
}

jobject
qrencode::encode(JNIEnv *env, const char *qrsource, unsigned int prescaler, unsigned int backColor,
                 unsigned int qrColor, jobject logo) {

    unsigned int unWidth, x, y, l, n;
    QRcode *pQRC;

    if (pQRC = QRcode_encodeString(qrsource, 0, QR_ECLEVEL_H, QR_MODE_8, 1)) {
        //根据输入大小自动放大缩放二维码
        prescaler = prescaler / pQRC->width;
        unWidth = pQRC->width;

        //create bitmap
        jobject jbitmap = createBitmap(env, unWidth * prescaler, unWidth * prescaler);

        AndroidBitmapInfo srcInfo;
        int result = 0;
        result = AndroidBitmap_getInfo(env, jbitmap, &srcInfo);
        if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
            if (LOGS_ENABLED) LOGE("%s", "AndroidBitmap_getInfo failed");
            return nullptr;
        }

        void *bitmapPixels;
        result = AndroidBitmap_lockPixels(env, jbitmap, &bitmapPixels);
        if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
            if (LOGS_ENABLED) LOGE("%s", "AndroidBitmap_lockPixels failed");
            return nullptr;
        }

        //default draw white,support exten other color.
        for (x = 0; x < srcInfo.width; x++) {
            for (y = 0; y < srcInfo.height; y++) {
                draw(srcInfo.stride, bitmapPixels, x, y, backColor);
            }
        }
        //convert to bitmap pixel.
        for (x = 0; x < unWidth; x++) {
            for (y = 0; y < unWidth; y++) {
                //write color.
                if (pQRC->data[x * pQRC->width + y] & 1) {
                    int sl = x * prescaler;
                    int el = x * prescaler + prescaler;
                    int sn = y * prescaler;
                    int en = y * prescaler + prescaler;
                    for (l = sl; l < el; l++) {
                        for (n = sn; n < en; n++) {
                            draw(srcInfo.stride, bitmapPixels, l, n, qrColor);
                        }
                    }
                }
            }
        }

        AndroidBitmap_unlockPixels(env, jbitmap);
        if (LOGS_ENABLED) LOGI("%s", "write successful!");
        decorate::insertLogo(env, &srcInfo, bitmapPixels, logo);
        // Free data
        QRcode_free(pQRC);
        return jbitmap;
    } else {
        if (LOGS_ENABLED) LOGE("%s", "NULL returned");
        return nullptr;
    }
}

jobject
qrencode::encode(JNIEnv *env, const char *qrsource, unsigned int prescaler, unsigned int backColor,
                 jobject overlay, jobject logo) {
    unsigned int unWidth, x, y, l, n;
    QRcode *pQRC;

    if (pQRC = QRcode_encodeString(qrsource, 0, QR_ECLEVEL_H, QR_MODE_8, 1)) {
        //根据输入大小自动放大缩放二维码
        prescaler = prescaler / pQRC->width;
        unWidth = pQRC->width;

        //create bitmap
        jobject jbitmap = createBitmap(env, unWidth * prescaler, unWidth * prescaler);

        AndroidBitmapInfo srcInfo;
        int result = 0;
        result = AndroidBitmap_getInfo(env, jbitmap, &srcInfo);
        if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
            if (LOGS_ENABLED) LOGE("%s", "AndroidBitmap_getInfo failed");
            return nullptr;
        }

        void *bitmapPixels;
        result = AndroidBitmap_lockPixels(env, jbitmap, &bitmapPixels);
        if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
            if (LOGS_ENABLED) LOGE("%s", "AndroidBitmap_lockPixels failed");
            return nullptr;
        }

        //////////////get overlay pixel//////////////////
        AndroidBitmapInfo overlayInfo;
        result = AndroidBitmap_getInfo(env, overlay, &overlayInfo);
        if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
            if (LOGS_ENABLED) LOGE("%s", "get overlay failed!");
        }
        void *overlayPixel;
        result = AndroidBitmap_lockPixels(env, overlay, &overlayPixel);
        if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
            if (LOGS_ENABLED) LOGE("%s", "lock overlay pixel failed");
        }

        //default draw white,support exten other color.
        for (x = 0; x < srcInfo.width; x++) {
            for (y = 0; y < srcInfo.height; y++) {
                draw(srcInfo.stride, bitmapPixels, x, y, backColor);
            }
        }

        //convert to bitmap pixel.
        for (x = 0; x < unWidth; x++) {
            for (y = 0; y < unWidth; y++) {
                //write color.
                if (pQRC->data[x * pQRC->width + y] & 1) {
                    int sl = x * prescaler;
                    int el = x * prescaler + prescaler;
                    int sn = y * prescaler;
                    int en = y * prescaler + prescaler;
                    for (l = sl; l < el; l++) {
                        for (n = sn; n < en; n++) {
                            if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
                                //use default color.
                                draw(srcInfo.stride, bitmapPixels, l, n, DEFAULT_QR_COLOR);
                            }else{
                                //write overlay pixel.
                                int color = ((int32_t *) overlayPixel)[overlayInfo.width * n + l];
                                uint32_t alpha = (color & 0xFF000000) >> 24;
                                uint32_t red = ((color & 0x00FF0000) >> 16);
                                uint32_t green = ((color & 0x0000FF00) >> 8);
                                uint32_t blue = color & 0x000000FF;
                                draw(srcInfo.stride, bitmapPixels, l, n,
                                     (alpha << 24) | (blue << 16) | (green << 8) | red);

                            }
                        }
                    }
                }
            }
        }

        AndroidBitmap_unlockPixels(env, overlay);
        AndroidBitmap_unlockPixels(env, jbitmap);
        if (LOGS_ENABLED) LOGI("%s", "write successful!");
        decorate::insertLogo(env, &srcInfo, bitmapPixels, logo);
        // Free data
        QRcode_free(pQRC);
        return jbitmap;
    } else {
        if (LOGS_ENABLED) LOGE("%s", "NULL returned");
        return nullptr;
    }
}

jobject qrencode::createBitmap(JNIEnv *env, uint32_t width, uint32_t height) {
    //find bitmap class.
    jclass jbitmapClass = env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapFunction = env->GetStaticMethodID(jbitmapClass,
                                                            "createBitmap",
                                                            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    //set Bitmap Config.
    jstring configName = env->NewStringUTF("ARGB_8888");
    jclass bitmapConfigClass = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID valueOfBitmapConfigFunction = env->GetStaticMethodID(
            bitmapConfigClass, "valueOf",
            "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");
    jobject bitmapConfig = env->CallStaticObjectMethod(bitmapConfigClass,
                                                       valueOfBitmapConfigFunction, configName);
    //call Bitmap.createBitmap()
    return env->CallStaticObjectMethod(jbitmapClass,
                                       createBitmapFunction,
                                       width,
                                       height, bitmapConfig);
}



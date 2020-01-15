//
// Created by ben622 on 2020/1/15.
//

#include "decorate.h"

void qrencode::decorate::insertLogo(JNIEnv *env, AndroidBitmapInfo *pixelInfo, void *pixels,
                                    jobject jlogo) {
    if (jlogo == NULL) {
        LOGE("%s", "insert logo is null");
        return;
    }
    AndroidBitmapInfo logoBitmapInfo;
    int result = 0;
    result = AndroidBitmap_getInfo(env, jlogo, &logoBitmapInfo);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("%s", "insert logo::AndroidBitmap_getInfo failed");
        return;
    }

    void *logoBitmapPixel;
    result = AndroidBitmap_lockPixels(env, jlogo, &logoBitmapPixel);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("%s", "insert logo::AndroidBitmap_lockPixels failed");
        return;
    }

    if (logoBitmapInfo.width > pixelInfo->width || logoBitmapInfo.height > pixelInfo->height) {
        LOGE("%s",
             "The width and height of the logo cannot be greater than the width and height of the QR code！");
        return;
    }

    LOGE("%s", "begin write logo pixel!");

    int cx = pixelInfo->width / 2;
    int cy = pixelInfo->height / 2;

    int sx = cx - logoBitmapInfo.width / 2;
    int sy = cy - logoBitmapInfo.height / 2;

    for (int y = sy; y < sy + logoBitmapInfo.height; y++) {
        for (int x = sx; x < sx + logoBitmapInfo.width; x++) {
            //write logo
            int color = ((int32_t *) logoBitmapPixel)[logoBitmapInfo.width * (y - sy) + (x - sx)];
            if(color ==0) continue;
            uint32_t alpha = (color & 0xFF000000) >> 24;
            uint32_t red = ((color & 0x00FF0000) >> 16);
            uint32_t green = ((color & 0x0000FF00) >> 8);
            uint32_t blue = color & 0x000000FF;
            draw(pixelInfo->stride, pixels, x, y,  (alpha << 24) | (red << 16) | (green << 8) | blue);
        }
    }

    AndroidBitmap_unlockPixels(env, jlogo);
}

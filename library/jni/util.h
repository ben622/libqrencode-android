//
// Created by ben622 on 2020/1/15.
//

#ifndef LIBQRENCODE_ANDROID_UTIL_H
#define LIBQRENCODE_ANDROID_UTIL_H

#include <jni.h>
#include <unistd.h>

static void draw_color(u_int32_t *pixel, u_int32_t color) {
    uint32_t alpha = (color & 0xFF000000) >> 24;
    uint32_t blue = (color & 0x00FF0000) >> 16;
    u_short green = (color & 0x0000FF00) >> 8;
    u_char red = color & 0x00000FF;
    *pixel = (alpha << 24) | (red << 16) | (green << 8) | blue;
}

static void draw(jint stride, void *pixels, u_short x, u_short y, u_int32_t color) {
    pixels = (char *) pixels + y * stride;
    u_int32_t *pixel = ((u_int32_t *) pixels) + x;
    draw_color(pixel, color);
}

#endif //LIBQRENCODE_ANDROID_UTIL_H

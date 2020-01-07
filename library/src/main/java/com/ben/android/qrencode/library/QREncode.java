package com.ben.android.qrencode.library;

import android.graphics.Bitmap;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.IntRange;

//
// Created by ben622 on 2020/1/2.
//
public class QREncode {
    private static final String TAG = "QREncode";

    static {
        System.loadLibrary("qrencode-android");
    }

    /**
     * @deprecated 以bmp格式输出在部分设备上可能存在兼容性问题，推荐使用{@link QREncode#encode(String, int, int)}
     * @param qrsource  二维码数据
     * @param prescaler 期望的二维码图片大小(像素点)，最终的大小可能小于该值。
     * @param output    输出文件，以bmp结尾
     * @return          大于等于0 successful
     */
    public static int encode(String qrsource, int prescaler, String output) {
        return encode(qrsource, prescaler, output, 0, 0, 0);
    }

    /**
     * @deprecated 以bmp格式输出在部分设备上可能存在兼容性问题，推荐使用{@link QREncode#encode(String, int, int)}
     * @param qrsource  二维码数据
     * @param prescaler 期望的二维码图片大小(像素点)，最终的大小可能小于该值。
     * @param output    输出文件，以bmp结尾
     * @param r         二维码颜色通道R
     * @param g         二维码颜色通道G
     * @param b         二维码颜色通道B
     * @return          大于等于0 successful
     */
    public static int encode(String qrsource, int prescaler, String output, @IntRange(from = 0, to = 255) int r, @IntRange(from = 0, to = 255) int g, @IntRange(from = 0, to = 255) int b) {
        if (TextUtils.isEmpty(output)) {
            Log.e(TAG, "This output file path cannot be null!");
            return -1;
        }
        if (!output.endsWith(".bmp")) {
            Log.e(TAG, "The output file suffix must be in .bmp!");
            return -1;
        }
        return nativeQREncode(qrsource, prescaler, output, r, g, b);
    }

    public static Bitmap encode(String qrsource, int prescaler, int color) {
        return nativeQREncode(qrsource, prescaler, color);
    }
    /**
     * @param qrsource  二维码数据
     * @param prescaler 期望的二维码图片大小，最终的大小可能小于该值。
     * @param output    输出文件，以bmp结尾
     * @param r         二维码颜色通道R
     * @param g         二维码颜色通道G
     * @param b         二维码颜色通道B
     * @return          大于等于0 successful
     */
    private native static int nativeQREncode(String qrsource, int prescaler, String output, @IntRange(from = 0, to = 255) int r, @IntRange(from = 0, to = 255) int g, @IntRange(from = 0, to = 255) int b);

    private native static Bitmap nativeQREncode(String qrsource, int prescaler,  int color);

}

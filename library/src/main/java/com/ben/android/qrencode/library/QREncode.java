package com.ben.android.qrencode.library;

import android.graphics.Bitmap;
import android.graphics.Color;
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
     * @param qrsource  二维码数据
     * @param prescaler 期望的二维码图片大小(像素点)，最终的大小可能小于该值。
     * @param output    输出文件，以bmp结尾
     * @return 大于等于0 successful
     * @deprecated 以bmp格式输出在部分设备上可能存在兼容性问题，推荐使用{@link QREncode#encode(String, int, int)}
     */
    public static int encode(String qrsource, int prescaler, String output) {
        return encode(qrsource, prescaler, output, 0, 0, 0);
    }

    /**
     * @param qrsource  二维码数据
     * @param prescaler 期望的二维码图片大小(像素点)，最终的大小可能小于该值。
     * @param output    输出文件，以bmp结尾
     * @param r         二维码颜色通道R
     * @param g         二维码颜色通道G
     * @param b         二维码颜色通道B
     * @return 大于等于0 successful
     * @deprecated 以bmp格式输出在部分设备上可能存在兼容性问题，推荐使用{@link QREncode#encode(String, int, int)}
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

    /**
     * @param qrsource  二维码数据
     * @param prescaler 期望的二维码图片大小，最终的大小可能小于该值。
     * @return 如果编码成功返回一个非空bitmap对象
     */
    public static Bitmap encode(String qrsource, int prescaler) {
        return nativeQREncode(qrsource, prescaler, Color.WHITE, Color.BLACK, null);
    }

    /**
     * @param qrsource  二维码数据
     * @param prescaler 期望的二维码图片大小，最终的大小可能小于该值。
     * @param color     二维码颜色，可以包含透明颜色通道
     * @return 如果编码成功返回一个非空bitmap对象
     */
    public static Bitmap encode(String qrsource, int prescaler, int color) {
        return nativeQREncode(qrsource, prescaler, Color.WHITE, color, null);
    }

    /**
     * @param qrsource        二维码数据
     * @param prescaler       期望的二维码图片大小，最终的大小可能小于该值。
     * @param backgroundColor 二维码背景颜色，可以包含透明颜色通道
     * @param color           二维码颜色，可以包含透明颜色通道
     * @return 如果编码成功返回一个非空bitmap对象
     */
    public static Bitmap encode(String qrsource, int prescaler, int backgroundColor, int color) {
        return nativeQREncode(qrsource, prescaler, backgroundColor, color, null);
    }

    /**
     * @param qrsource  二维码数据
     * @param prescaler 期望的二维码图片大小，最终的大小可能小于该值。
     * @param logo      在二维码中添加logo.
     * @return 如果编码成功返回一个非空bitmap对象
     */
    public static Bitmap encode(String qrsource, int prescaler, Bitmap logo) {
        return nativeQREncode(qrsource, prescaler, Color.WHITE, Color.BLACK, logo);
    }

    /**
     * 创建一个带Logo的二维码
     * @param qrsource        二维码数据
     * @param prescaler       期望的二维码图片大小，最终的大小可能小于该值。
     * @param backgroundColor 二维码背景颜色，可以包含透明颜色通道
     * @param color           二维码颜色，可以包含透明颜色通道
     * @param logo            在二维码中添加logo.
     * @return 如果编码成功返回一个非空bitmap对象
     */
    public static Bitmap encode(String qrsource, int prescaler, int backgroundColor, int color, Bitmap logo) {
        return nativeQREncode(qrsource, prescaler, backgroundColor, color, logo);
    }

    /**
     * 使用图片填充二维码
     * @param qrsource        二维码数据
     * @param prescaler       期望的二维码图片大小，最终的大小可能小于该值。
     * @param qroverlay       填充二维码的位图使用{@link QREncodeUtilities#createQROverlay(Bitmap, int)}创建
     * @return 如果编码成功返回一个非空bitmap对象
     */
    public static Bitmap encodeOverlay(String qrsource, int prescaler,Bitmap qroverlay) {
        return nativeQREncode(qrsource, prescaler, Color.WHITE, qroverlay, null);
    }

    /**
     * 创建一个带Logo的二维码
     * @param qrsource        二维码数据
     * @param prescaler       期望的二维码图片大小，最终的大小可能小于该值。
     * @param backgroundColor 二维码背景颜色，可以包含透明颜色通道
     * @param qroverlay       填充二维码的位图使用{@link QREncodeUtilities#createQROverlay(Bitmap, int)}创建
     * @param logo            在二维码中添加logo.
     * @return 如果编码成功返回一个非空bitmap对象
     */
    public static Bitmap encodeOverlay(String qrsource, int prescaler,int backgroundColor,Bitmap qroverlay,Bitmap logo) {
        return nativeQREncode(qrsource, prescaler, backgroundColor, qroverlay, logo);
    }
    /**
     * @param qrsource  二维码数据
     * @param prescaler 期望的二维码图片大小，最终的大小可能小于该值。
     * @param output    输出文件，以bmp结尾
     * @param r         二维码颜色通道R
     * @param g         二维码颜色通道G
     * @param b         二维码颜色通道B
     * @return 大于等于0 successful
     */
    public native static int nativeQREncode(String qrsource, int prescaler, String output, @IntRange(from = 0, to = 255) int r, @IntRange(from = 0, to = 255) int g, @IntRange(from = 0, to = 255) int b);

    /**
     * @param qrsource        二维码数据
     * @param prescaler       期望的二维码图片大小，最终的大小可能小于该值。
     * @param backgroundColor 二维码背景颜色，可以包含透明颜色通道
     * @param color           二维码颜色，可以包含透明颜色通道
     * @return 如果编码成功返回一个非空bitmap对象
     */
    public native static Bitmap nativeQREncode(String qrsource, int prescaler, int backgroundColor, int color, Bitmap logo);


    public native static Bitmap nativeQREncode(String qrsource, int prescaler, int backgroundColor, Bitmap overlay, Bitmap logo);


}

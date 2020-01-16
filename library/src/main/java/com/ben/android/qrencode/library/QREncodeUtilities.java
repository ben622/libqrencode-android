package com.ben.android.qrencode.library;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.util.Log;
import android.widget.Toast;

//
// Created by ben622 on 2020/1/16.
//
public class QREncodeUtilities {
    /**
     * 创建一个使用图片填充二维码的位图.
     * @param overlay
     * @param qrsize
     * @return
     */
    public static Bitmap createQROverlay(Bitmap overlay, int qrsize) {
        if (overlay == null) return null;
        int width = overlay.getWidth();
        int height = overlay.getHeight();
        Matrix matrix = new Matrix();
        matrix.setScale((float)qrsize / width,(float)qrsize / height);
        return Bitmap.createBitmap(overlay, 0, 0, width, height, matrix, false);
    }
}

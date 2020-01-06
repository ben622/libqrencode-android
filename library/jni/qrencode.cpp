//
// Created by ben622 on 2020/1/2.
//

#include "qrencode.hpp"

jint qrencode::encode(const char *qrsource, unsigned int prescaler, const char *output) {
    return encode(qrsource, prescaler, output, 0, 0, 0);
}

jint qrencode::encode(const char *qrsource, unsigned int prescaler, const char *output, unsigned int r,
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
            if (LOGS_ENABLED) LOGE("%s","Out of memory");
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
            if (LOGS_ENABLED) LOGE("%s","Unable to open file");
            return -1;
        }

        // Free data
        free(pRGBData);
        QRcode_free(pQRC);
        if (LOGS_ENABLED) LOGI("qrencode successful,outputfile:%s",output);
    } else {
        if (LOGS_ENABLED) LOGE("%s","NULL returned");
        return -1;
    }
    return 0;
}

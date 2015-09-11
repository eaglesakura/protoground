#include    "RawImage.h"
#include    "es/internal/protoground-internal.hpp"

namespace es {

RawImage::RawImage() {
    format = PixelFormat_NativeRGBA;
    width = height = 0;
}

RawImage::~RawImage() {
}

/**
 * フォーマットを指定してピクセルバッファを生成する
 */
void RawImage::alloc(const PixelFormat_e format, const uint width, const uint height) {
    this->format = format;
    this->width = width;
    this->height = height;
    // ピクセルバッファの取得を行う
    {
        const int size = getBufferSize(format, width, height);
        if (this->pBuffer.length < size) {
            eslog("realloc buffer %d bytes", size);
            this->pixels.alloc(size);
            pBuffer = pixels.iterator();
        } else {
            eslog("to allocated buffer %x -> %d bytes", pBuffer.ptr, pBuffer.length);
        }
    }
}

/**
 * ピクセルバッファの格納先を外部から指定する
 */
void RawImage::setBuffer(const void *buffer, const uint length) {
    pBuffer.ptr = (uint8_t *) buffer;
    pBuffer.length = length;
    pixels.clear();
}

/**
 * y0行とy1行のピクセルを入れ替える
 */
void RawImage::swapLineY(const uint y0, const uint y1) {

}

/**
 * 縦方向に反転する。
 * テクスチャからキャプチャした画素情報の天地反転用
 */
void RawImage::revertImageV() {

}

}

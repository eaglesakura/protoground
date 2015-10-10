#pragma once

#include    "es/memory/SafeArray.hpp"
#include    "es/graphics/PixelFormat.hpp"

namespace es {

class RawImage : public Object {
public:
    RawImage();

    virtual ~RawImage();

    /**
     * フォーマットを指定してピクセルバッファを生成する
     */
    virtual void alloc(const PixelFormat_e format, const unsigned width, const unsigned height);

    /**
     * ピクセルバッファの格納先を外部から指定する
     */
    virtual void setBuffer(const void *buffer, const unsigned length);

    /**
     * 縦方向に反転する。
     * テクスチャからキャプチャした画素情報の天地反転用
     */
    void revertImageV();

    /**
     * ピクセルバッファの長さを取得する
     */
    unsigned getPixelBufferLength() const {
        return pBuffer.length;
    }

    /**
     * ピクセルバッファのフォーマットを取得する
     */
    PixelFormat_e getPixelFormat() const {
        return format;
    }

    /**
     * 1ピクセルの容量を取得するう
     */
    unsigned getPixelBytes() const {
        return Pixel::getPixelBytes(getPixelFormat());
    }

    void *getPixels() const {
        return pBuffer.ptr;
    }

    /**
     * 指定したY位置のピクセルバッファを取得する
     */
    void *getPixels(const unsigned y) {
        return ((uint8_t *) getPixels()) + (y * getWidth() * getPixelBytes());
    }

    /**
     * ピクセルバッファ幅を取得する
     */
    unsigned getWidth() const {
        return width;
    }

    /**
     * ピクセルバッファ高を取得する
     */
    unsigned getHeight() const {
        return height;
    }

    static unsigned getBufferSize(const PixelFormat_e format, const unsigned width, const unsigned height) {
        return Pixel::getPixelBytes(format) * width * height;
    }

private:
    /**
     * 確保されたピクセルバッファ
     */
    safe_array<uint8_t> pixels;

    /**
     * 直接アクセス用のバッファ
     */
    unsafe_array<uint8_t> pBuffer;

    /**
     * ピクセルバッファフォーマット
     */
    PixelFormat_e format;

    /**
     * 画像幅
     */
    unsigned width;

    /**
     * 画像高
     */
	unsigned height;

    /**
     * y0行とy1行のピクセルを入れ替える
     */
    void swapLineY(const unsigned y0, const unsigned y1);
};


}

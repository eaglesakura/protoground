#pragma once

#include    "es/protoground.hpp"
#include    "es/memory/Buffer.hpp"

namespace es {

/**
 * ピクセルフォーマットを指定する
 */
enum PixelFormat_e {
    /**
     * 2byte
     * GL_UNSIGNED_SHORT_5_6_5
     */
            PixelFormat_RGB565,

    /**
     * 2byte
     * GL_UNSIGNED_SHORT_5_5_5_1
     */
            PixelFormat_RGBA5551,

    /**
     * 3byte
     * GL_UNSIGNED_BYTE
     */
            PixelFormat_RGB888,

    /**
     * 4byte
     * GL_UNSIGNED_BYTE
     */
            PixelFormat_RGBA8888,

    /**
     * 4byte
     * GL_UNSIGNED_BYTE
     */
            PixelFormat_BGRA8888,

    /**
     * 2byte
     * GL_HALF_FLOAT
     */
            PixelFormat_LuminanceF16,

    /**
     * 8byte
     * GL_HALF_FLOAT
     */
            PixelFormat_RGBA_F16,

    /**
     * 1byte
     * GL_ALPHA
     */
            PixelFormat_A8,

    /**
     * 2byte
     * Depth
     */
            PixelFormat_Depth,

    /**
     * 輝度限定テクスチャ
     */
            PixelFormat_Luminance8,

    /**
     * 単色テクスチャ
     */
            PixelFormat_R8,

    /**
     * RGBA各要素を含み、プラットフォームに最適化した状態でロードする
     * 各1byteは保証するが、RGBA並び順は保証しない。
     *
     * Android::高速に読み込めるため、その順で格納される。
     * 4byte
     * GL_BGRA_EXT
     * GL_UNSIGNED_BYTE
     */
#ifdef  BUILD_Android
    PixelFormat_NativeRGBA = PixelFormat_BGRA8888,
#else
    PixelFormat_NativeRGBA = PixelFormat_RGBA8888,
#endif
};

class Pixel {
public:

    /**
     * フォーマットごとの1ピクセルサイズを取得する
     */
    static const unsigned getPixelBytes(const PixelFormat_e fmt);

    /**
     * 指定したピクセルフォーマットで規定のピクセル数分のバッファを作成する
     */
    static ByteBuffer createPixelBuffer(const PixelFormat_e fmt, const unsigned pixels);

    /**
     * RGB888のポインタをdstへピクセル情報をコピーする。
     * 同じポインタの場合は、何も行わない。
     */
    static void copyRGB888Pixels(const uint8_t *src_rgb888, const PixelFormat_e dstFormat, uint8_t *dst, const unsigned _pixels);

    /**
     * RGB888のデータを変換コピーする
     * 同じポインタの場合何も行わない。
     * src_rgb888は隙間なくピクセルが並んでいることを前提とする。
     *
     * stride > widthである必要がある。
     * stride * heightのピクセル数が必要になる。
     */
    static void copyRGB888Pixels(const uint8_t *src_rgb888, const PixelFormat_e dstFormat, uint8_t *dst, const unsigned width, const unsigned height, const unsigned stride);

    /**
     * RGB888のポインタをdstへピクセル情報をコピーする。
     * 同じポインタの場合は、何も行わない。
     */
    static void copyRGBA8888Pixels(const uint8_t *src_rgba8888, const PixelFormat_e dstFormat, uint8_t *dst, const unsigned _pixels);

    /**
     * RGBA8888のデータを変換コピーする
     * 同じポインタの場合何も行わない。
     * src_rgba8888は隙間なくピクセルが並んでいることを前提とする。
     *
     * stride > widthである必要がある。
     * stride * heightのピクセル数が必要になる。
     */
    static void copyRGBA8888Pixels(const uint8_t *src_rgba8888, const PixelFormat_e dstFormat, uint8_t *dst, const unsigned width, const unsigned height, const unsigned stride);

    /**
     * RGB888のポインタをdstへピクセル情報をコピーする。
     * 同じポインタの場合は、何も行わない。
     */
    static void copyBGRA8888Pixels(const uint8_t *src_bgra8888, const PixelFormat_e dstFormat, uint8_t *dst, const unsigned _pixels);

    /**
     * GLESで使用されるピクセルフォーマットへ変換する
     *
     * GL_RGBA等に変換される
     */
    static uint32_t toGLPixelFormat(const PixelFormat_e format);

    /**
     * GLESで使用されるデータタイプへ変換する
     *
     * GL_UNSIGNED_BYTE等に変換される
     */
    static uint32_t toGLPixelDataType(const PixelFormat_e format);
};

}

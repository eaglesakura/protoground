#include "es/graphics/PixelFormat.hpp"
#include "es/internal/protoground-internal.hpp"
namespace es {

void Pixel::copyRGB888Pixels(const uint8_t *src_rgb888, const PixelFormat_e dstFormat, uint8_t *dst, const unsigned _pixels) {
    if (src_rgb888 == dst) {
        return;
    }

    // 残ピクセル数
    unsigned pixels = _pixels;

    switch (dstFormat) {
        case PixelFormat_RGB565: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                unsigned r = src_rgb888[0] & 0xff;
                unsigned g = src_rgb888[1] & 0xff;
                unsigned b = src_rgb888[2] & 0xff;

                (*p) = ((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3));
                src_rgb888 += 3;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGBA5551: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                unsigned r = src_rgb888[0] & 0xff;
                unsigned g = src_rgb888[1] & 0xff;
                unsigned b = src_rgb888[2] & 0xff;
                (*p) = ((r >> 3) << 12) | ((g >> 3) << 6) | ((b >> 3) << 1) | 1;
                src_rgb888 += 3;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGB888: {
            memcpy(dst, src_rgb888, pixels * 3);
        }
            break;
        case PixelFormat_RGBA8888: {
            while (pixels) {

                dst[0] = src_rgb888[0];
                dst[1] = src_rgb888[1];
                dst[2] = src_rgb888[2];
                dst[3] = 0xFF;

                src_rgb888 += 3;
                dst += 4;
                --pixels;
            }
        }
            break;
        default:
            assert(false); // not support!!
            break;
    }
}

void Pixel::copyRGB888Pixels(const uint8_t *src_rgb888, const PixelFormat_e dstFormat, uint8_t *dst, const unsigned width, const unsigned height, const unsigned stride) {
    if (width == stride) {
        copyRGB888Pixels(src_rgb888, dstFormat, dst, width * height);
    } else {
        // 1ラインごとに分割してコピーする
        int line = height;
        uint8_t *dstLineBuffer = dst;
        uint8_t *srcLineBUffer = (uint8_t *) src_rgb888;
        const unsigned pixelSize = getPixelBytes(dstFormat);
        while (line > 0) {
            copyRGB888Pixels(srcLineBUffer, dstFormat, dstLineBuffer, width);

            srcLineBUffer += (width * 3);
            dstLineBuffer += (stride * pixelSize);
            --line;
        }
    }
}

void Pixel::copyBGRA8888Pixels(const uint8_t *src_bgra8888, const PixelFormat_e dstFormat, uint8_t *dst, const unsigned _pixels) {
    if (src_bgra8888 == dst) {
        return;
    }

    // 残ピクセル数
    unsigned pixels = _pixels;

    const int pixel_size = 4;

    switch (dstFormat) {
        case PixelFormat_RGB565: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                const unsigned r = src_bgra8888[2] & 0xff;
                const unsigned g = src_bgra8888[1] & 0xff;
                const unsigned b = src_bgra8888[0] & 0xff;

                (*p) = ((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3));
                src_bgra8888 += pixel_size;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGBA5551: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                const unsigned r = src_bgra8888[2] & 0xff;
                const unsigned g = src_bgra8888[1] & 0xff;
                const unsigned b = src_bgra8888[0] & 0xff;
                const unsigned a = (src_bgra8888[3] & 0xff) > 0 ? 1 : 0;
                (*p) = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | a;
                src_bgra8888 += pixel_size;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGB888: {
            while (pixels) {

                dst[0] = src_bgra8888[2];
                dst[1] = src_bgra8888[1];
                dst[2] = src_bgra8888[0];

                src_bgra8888 += pixel_size;
                dst += 3;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGBA8888: {
            while (pixels) {

                dst[0] = src_bgra8888[2];
                dst[1] = src_bgra8888[1];
                dst[2] = src_bgra8888[0];
                dst[3] = src_bgra8888[3];

                src_bgra8888 += pixel_size;
                dst += 4;
                --pixels;
            }
        }
            break;
        default:
            eslog("unknown dst format(%d)", dstFormat);
            assert(false); // not support!!
            break;
    }
}

void Pixel::copyRGBA8888Pixels(const uint8_t *src_rgba8888, const PixelFormat_e dstFormat, uint8_t *dst, const unsigned _pixels) {
    if (src_rgba8888 == dst) {
        return;
    }

    // 残ピクセル数
    unsigned pixels = _pixels;

    const int pixel_size = 4;

    switch (dstFormat) {
        case PixelFormat_RGB565: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                const unsigned r = src_rgba8888[0] & 0xff;
                const unsigned g = src_rgba8888[1] & 0xff;
                const unsigned b = src_rgba8888[2] & 0xff;

                (*p) = ((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3));
                src_rgba8888 += pixel_size;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGBA5551: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                const unsigned r = src_rgba8888[0] & 0xff;
                const unsigned g = src_rgba8888[1] & 0xff;
                const unsigned b = src_rgba8888[2] & 0xff;
                const unsigned a = (src_rgba8888[3] & 0xff) > 0 ? 1 : 0;
                (*p) = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | a;
                src_rgba8888 += pixel_size;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGB888: {
            while (pixels) {

                dst[0] = src_rgba8888[0];
                dst[1] = src_rgba8888[1];
                dst[2] = src_rgba8888[2];

                src_rgba8888 += pixel_size;
                dst += 3;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGBA8888: {
            memcpy(dst, src_rgba8888, pixels * pixel_size);
        }
            break;

        default:
            eslog("unknown dst format(%d)", dstFormat);
            assert(false); // not support!!
            break;
    }
}

void Pixel::copyRGBA8888Pixels(const uint8_t *src_rgba8888, const PixelFormat_e dstFormat, uint8_t *dst, const unsigned width, const unsigned height, const unsigned stride) {
    if (width == stride) {
        copyRGB888Pixels(src_rgba8888, dstFormat, dst, width * height);
    } else {
        // 1ラインごとに分割してコピーする
        int line = height;
        uint8_t *dstLineBuffer = dst;
        uint8_t *srcLineBUffer = (uint8_t *) src_rgba8888;
        while (line > 0) {
            copyRGB888Pixels(srcLineBUffer, dstFormat, dstLineBuffer, width);

            srcLineBUffer += (width * 4);
            dstLineBuffer += (stride * 4);
            --line;
        }
    }
}

const unsigned Pixel::getPixelBytes(const PixelFormat_e fmt) {
    constexpr const unsigned size[] = {
            //
            2,// RGB565
            2, // RGBA5551
            3, // RGB888
            4, // RGBA8888
            4, // BGRA8888
            2, // LuminanceF16
            8, // RGBA_F16
            1, // A8
            1, // Luminance8
    };

    return size[fmt];
}

ByteBuffer Pixel::createPixelBuffer(const PixelFormat_e fmt, const unsigned pixels) {
    unsigned bytes = getPixelBytes(fmt);
    return Buffer::create(bytes * pixels);
}


}
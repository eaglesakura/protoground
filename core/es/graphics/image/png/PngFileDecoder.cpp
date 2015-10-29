#include "PngFileDecoder.h"
#include <png.h>
#include "es/internal/protoground-internal.hpp"
#include "es/graphics/image/IImageDecodeCallback.hpp"
#include "es/math/Math.hpp"

namespace es {

PngFileDecoder::PngFileDecoder() {

}

PngFileDecoder::~PngFileDecoder() {

}

namespace internal {
struct ImageBufferReader {
    unsafe_array<uint8_t> preLoad;
    sp<IAsset> asset;
    bool readError = false;

    /**
     * バッファを読み出す
     */
    bool loadBuffer(uint8_t *result, unsigned length) {
        if (preLoad) {
            // 読み込み可能な長さ分だけ読み込む
            int read = std::min(preLoad.length, (int) length);
            memcpy(result, preLoad.ptr, read);

            // 読み込んだ分だけオフセットして、残りの長さを縮める
            result += read;
            preLoad += read;
            length -= read;
        }

        // アセットから必要な長さを読み出す
        if (length) {
            unsafe_array<uint8_t> buffer = asset->read(length);
            if (buffer.length < length) {
                // 容量が足りない！
                memcpy(result, buffer.ptr, buffer.length);
                readError = true;
                return false;
            } else {
                assert(buffer.length == length);
                // 十分な容量を読み込めた
                memcpy(result, buffer.ptr, length);
            }
        }

        return true;
    }
};
}

static void pngReadBuffer(png_structp png, png_bytep result, png_size_t readSize) {
//    eslog("PNG read request(%d bytes)", readSize);
    internal::ImageBufferReader *reader = (internal::ImageBufferReader *) png_get_io_ptr(png);
    if (!reader->loadBuffer((uint8_t *) result, readSize)) {
        eslog("PNG BufferOver!!");
    }
}

bool PngFileDecoder::load(std::shared_ptr<IAsset> asset, selection_ptr<IImageDecodeCallback> listener) {
    IImageDecodeCallback::ImageInfo info;
    info.dstPixelFormat = pixelConvert;

    internal::ImageBufferReader reader;
    reader.asset = asset;
    reader.preLoad = unsafe_array<uint8_t>(readedBuffer.get(), readedBuffer.length());

    struct PngData {

        png_structp png = nullptr;

        png_infop info = nullptr;

        ~PngData() {
            png_destroy_read_struct(&png, &info, nullptr);
            assert(!png);
            assert(!info);
        }
    } data;

    // PNG初期化
    // TODO Error Handleを確定させる
    // TODO 途中キャンセル時のメモリ管理
    data.png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    assert(data.png);
    data.info = png_create_info_struct(data.png);
    assert(data.info);

    if (setjmp(png_jmpbuf(data.png))) {
        eslog("setjmp failed");
        listener->onImageDecodeFinished(&info, IImageDecodeCallback::ImageDecodeResult_FormatError);
        return false;
    }

    png_set_read_fn(data.png, (void *) (&reader), pngReadBuffer);

    // info
    png_read_info(data.png, data.info);

    // 読み込みに必要なテンポラリを生成する
    info.srcWidth = png_get_image_width(data.png, data.info);
    info.srcHeight = png_get_image_height(data.png, data.info);
    info.format = IImageDecodeCallback::ImageFormat_PNG;
    int depth = png_get_bit_depth(data.png, data.info);
    int rowBytes = png_get_rowbytes(data.png, data.info);
    int perPixelBytes = rowBytes / info.srcWidth;
    {

        if (!info.srcWidth || !info.srcHeight) {
            listener->onImageDecodeFinished(&info, IImageDecodeCallback::ImageDecodeResult_FormatError);
            return false;
        }

        eslog("PNG [%d x %d] %d bits = %d bytes/pixel [row = %d bytes]", info.srcWidth, info.srcHeight, depth, perPixelBytes, rowBytes);
        eslog("PNG read[%d] Once Lines", onceReadLines);


        if (depth == 8) {
            if (perPixelBytes == 3) {
                info.srcPixelFormat = PixelFormat_RGB888;
            } else if (perPixelBytes == 4) {
                info.srcPixelFormat = PixelFormat_RGBA8888;
            } else {
                eslog("Not Support PNG depth bits...");
                listener->onImageDecodeFinished(&info, IImageDecodeCallback::ImageDecodeResult_FormatError);
                return false;
            }
        } else {
            eslog("Not Support PNG depth bits...");
            listener->onImageDecodeFinished(&info, IImageDecodeCallback::ImageDecodeResult_FormatError);
            return false;
        }

        if (convertPot) {
            info.dstWidth = toPowerOfTwo(info.srcWidth);
            info.dstHeight = toPowerOfTwo(info.srcHeight);
        } else {
            info.dstWidth = info.srcWidth;
            info.dstHeight = info.srcHeight;
        }

        // インフォメーションをコールバックする
        listener->onImageInfoDecoded(&info);
    }


    // 一度に読み込む行数を確定する
    int onceReadLines = std::min((int) info.srcHeight, (int) this->onceReadLines);
    if (onceReadLines == 0) {
        onceReadLines = info.srcHeight;
    }

    std::vector<uint8_t> readCacheBuffer;
    ByteBuffer convertBuffer;

    if (info.srcPixelFormat != this->pixelConvert) {
        // 変換先の画素を生成する
        convertBuffer = Pixel::createPixelBuffer(pixelConvert, Pixel::getPixelBytes(pixelConvert) * info.dstWidth * onceReadLines);
    }

    std::vector<void *> rowHeaders(onceReadLines);

    if (convertBuffer.empty()) {
        // 変換予定の無い場合は隙間設定をしながら取得する
        util::valloc(&readCacheBuffer, perPixelBytes * info.dstWidth * onceReadLines, false);
        for (int i = 0; i < onceReadLines; ++i) {
            rowHeaders[i] = (void *) util::asPointer(readCacheBuffer, (perPixelBytes * info.dstWidth) * i);
        }
    } else {
        // 変換予定の場合は隙間なくバッファを取得して変換用メモリも設定する
        util::valloc(&readCacheBuffer, perPixelBytes * info.srcWidth * onceReadLines, false);
        for (int i = 0; i < onceReadLines; ++i) {
            rowHeaders[i] = (void *) util::asPointer(readCacheBuffer, (perPixelBytes * info.srcWidth) * i);
        }
    }

    int lines = info.srcHeight;
    while (lines) {
        uint32_t reading = std::min((uint32_t) lines, (uint32_t) onceReadLines);
//        eslog("PNG read[%d] Lines", reading);
        png_read_rows(data.png, NULL, (png_bytepp) util::asPointer(rowHeaders), reading);
        lines -= reading;
        if (reader.readError) {
            listener->onImageDecodeFinished(&info, IImageDecodeCallback::ImageDecodeResult_IOError);
            return false;
        } else if (listener->isImageDecodeCancel()) {
            listener->onImageDecodeFinished(&info, IImageDecodeCallback::ImageDecodeResult_Canceled);
            return false;
        }

        // バッファをリスナに伝える
        if (convertBuffer.empty()) {
            listener->onImageLineDecoded(&info, unsafe_array<uint8_t>(util::asPointer(readCacheBuffer), reading * rowBytes), reading);
        } else {
//            eslog("PNG Convert[%d] -> [%d]", info.srcPixelFormat, pixelConvert);
            if (info.srcPixelFormat == PixelFormat_RGB888) {
                Pixel::copyRGB888Pixels(
                        util::asPointer(readCacheBuffer),
                        pixelConvert, convertBuffer.get(),
                        info.srcWidth, reading, info.dstWidth
                );
            } else if (info.srcPixelFormat == PixelFormat_RGBA8888) {
                Pixel::copyRGBA8888Pixels(
                        util::asPointer(readCacheBuffer),
                        pixelConvert, convertBuffer.get(),
                        info.srcWidth, reading, info.dstWidth
                );
            } else {
                eslog("PNG Convert not support...");
                listener->onImageDecodeFinished(&info, IImageDecodeCallback::ImageDecodeResult_FormatError);
                return false;
            }
            listener->onImageLineDecoded(&info, unsafe_array<uint8_t>(convertBuffer.get(), perPixelBytes * info.dstWidth * reading), reading);
        }
    }

    listener->onImageDecodeFinished(&info, IImageDecodeCallback::ImageDecodeResult_Success);
    return true;
}

void PngFileDecoder::setReadedBuffer(const unsafe_array<uint8_t> &buffer) {
    this->readedBuffer = Buffer::clone(buffer);
}

void PngFileDecoder::setOnceReadHeight(const unsigned heightPixels) {
    this->onceReadLines = heightPixels;
}

void PngFileDecoder::setConvertPixelFormat(const PixelFormat_e format) {
    this->pixelConvert = format;
}

void PngFileDecoder::setConvertPot(bool set) {
    this->convertPot = set;
}

bool PngFileDecoder::isPngFile(const unsafe_array<uint8_t> &sign) {
    if (png_sig_cmp(sign.ptr, 0, sign.length)) {
        eslog("png_sig_cmp failed");
        return false;
    } else {
        return true;
    }
}
}
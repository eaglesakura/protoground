#pragma once

#include "estest/protoground-test.hpp"
#include "es/graphics/image/png/PngFileDecoder.h"
#include "es/graphics/image/IImageDecodeCallback.hpp"

namespace es {
namespace test {


namespace internal {

std::shared_ptr<IImageDecodeCallback> newSimpleImageListener() {
    class PngImageListener : public IImageDecodeCallback {
        ImageInfo info;
        bool infoReceived = false;
        int readedLines = 0;
    public:

        PngImageListener() { }

        virtual ~PngImageListener() { }

        /**
         * 画像情報を読み込んだ
         */
        virtual void onImageInfoDecoded(const ImageInfo *info) {
            ASSERT_NE(info, nullptr);
            ASSERT_TRUE(info->srcWidth > 0);
            ASSERT_TRUE(info->srcHeight > 0);

            this->info = *info;
            infoReceived = true;
        }

        /**
         * 画像を指定行読み込んだ
         *
         * 引数lineは使いまわされる可能性があるため、内部的にテクスチャコピー等を行うこと。
         */
        virtual void onImageLineDecoded(const ImageInfo *info, const unsafe_array<uint8_t> pixels, const unsigned height) {
            ASSERT_TRUE(pixels.length > 0);
            ASSERT_TRUE(height > 0);
            ASSERT_TRUE(infoReceived);
            readedLines += height;
            ASSERT_TRUE(readedLines <= (int) info->srcHeight);
        }

        /**
         * 画像のデコードをキャンセルする場合はtrue
         */
        virtual bool isImageDecodeCancel() {
            return false;
        }

        /**
         * デコードが完了した
         */
        virtual void onImageDecodeFinished(const ImageInfo *info, const ImageDecodeResult_e result) {
            ASSERT_EQ(result, ImageDecodeResult_Success);
            ASSERT_EQ(info->srcHeight, readedLines);
        }
    };

    sp<IImageDecodeCallback> result(new PngImageListener());
    return result;
}

}

/**
 * 正方形PowerOfTwo PNG画像を読み込む
 */
TEST(LibPngDecoderTest, DecodeSquarePot_dstRGB8) {
    sp<IAsset> asset = IProcessContext::getInstance()->getAssetManager()->load("png/square-pot.png");
    ASSERT_TRUE((bool) asset);

    PngFileDecoder decoder;
    decoder.setOnceReadHeight(25);
    decoder.setConvertPixelFormat(PixelFormat_RGB888);
    ASSERT_TRUE(decoder.load(asset, internal::newSimpleImageListener()));
}

/**
 * 正方形PowerOfTwo PNG画像を読み込む
 */
TEST(LibPngDecoderTest, DecodeSquarePot_dstRGBA8) {
    sp<IAsset> asset = IProcessContext::getInstance()->getAssetManager()->load("png/square-pot.png");
    ASSERT_TRUE((bool) asset);

    PngFileDecoder decoder;
    decoder.setOnceReadHeight(25);
    decoder.setConvertPixelFormat(PixelFormat_RGBA8888);
    ASSERT_TRUE(decoder.load(asset, internal::newSimpleImageListener()));
}

}
}
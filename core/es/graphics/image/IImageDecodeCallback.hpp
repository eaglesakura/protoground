#pragma once

#include "es/graphics/PixelFormat.hpp"

namespace es {

/**
 * フォーマットに依存しない画素情報の受け取りを行う
 */
/**
 * 画像読み込み時にコールバックを受け取る
 */
class IImageDecodeCallback {
public:
    enum ImageFormat_e {
        /**
         * PNGファイル
         */
                ImageFormat_PNG,
        /**
         * TrueType Font
         */
                ImageFormat_TrueType,
        /**
         * 未知のファイル
         */
                ImageFormat_Unknown,
    };

    /**
     * デコード結果を受け取る
     */
    enum ImageDecodeResult_e {
        /**
         * キャンセルされた
         */
                ImageDecodeResult_Canceled = -1,

        /**
         * 正常に成功した
         */
                ImageDecodeResult_Success = 0,

        /**
         * 読み込み・書き込みでエラーが発生した
         */
                ImageDecodeResult_IOError,

        /**
         * 画像フォーマットに問題がある
         */
                ImageDecodeResult_FormatError,

        /**
         * 何らかの原因でエラーが発生した
         */
                ImageDecodeResult_UnknownError,
    };

    /**
     * 画像の基本情報
     */
    struct ImageInfo {
        /**
         * 幅
         */
        uint16_t srcWidth = 0;

        /**
         * 高さ
         */
        uint16_t srcHeight = 0;

        /**
         * デコード後の幅
         */
        uint16_t dstWidth = 0;

        /**
         * デコード後の高さ
         */
        uint16_t dstHeight = 0;

        /**
         * 画像のデコードフォーマット
         */
        ImageFormat_e format = ImageFormat_Unknown;

        /**
         * 元画像ピクセルフォーマット
         */
        PixelFormat_e srcPixelFormat = PixelFormat_RGBA8888;

        /**
         * 変換されたピクセルフォーマット
         */
        PixelFormat_e dstPixelFormat = PixelFormat_RGBA8888;
    };

    /**
     * 画像情報を読み込んだ
     *
     * infoの所有権はDecoderにあるため、ポインタを保存してはならない。
     */
    virtual void onImageInfoDecoded(const ImageInfo *info) = 0;

    /**
     * 画像を指定行読み込んだ
     *
     * 引数lineは使いまわされる可能性があるため、内部的にテクスチャコピー等を行うこと。
     */
    virtual void onImageLineDecoded(const ImageInfo *info, const unsafe_array<uint8_t> pixels, const uint height) = 0;

    /**
     * 画像のデコードをキャンセルする場合はtrue
     */
    virtual bool isImageDecodeCancel() = 0;

    /**
     * デコードが完了した
     */
    virtual void onImageDecodeFinished(const ImageInfo *info, const ImageDecodeResult_e result) = 0;
};


}
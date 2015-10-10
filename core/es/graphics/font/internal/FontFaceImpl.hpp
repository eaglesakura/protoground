#pragma once

#include "es/internal/protoground-internal.hpp"
#include <es/graphics/PixelFormat.hpp>
#include "es/asset/IAsset.hpp"
#include "es/graphics/font/FontFace.h"
#include "es/graphics/font/internal/FreetypeLibrary.hpp"
#include "es/graphics/font/internal/FontCharactorImpl.hpp"
#include "es/system/OptionalMutex.h"

namespace es {
namespace internal {

class FontFaceImpl : public FontFace {
    std::shared_ptr<FreetypeLibrary> library;

    std::shared_ptr<IAsset> asset;

    FT_Face face;

    OptionalMutex mutex;

public:
    FontFaceImpl(std::shared_ptr<FreetypeLibrary> newLibrary,
                 const FT_Face newFrace,
                 std::shared_ptr<IAsset> newAsset) :
            library(newLibrary), face(newFrace), asset(newAsset) {
        assert(this->face);
        assert(this->library);
        assert(this->asset);
        setSize(64, 64);
    }


    virtual ~FontFaceImpl() {
        FT_Done_Face(face);
        face = nullptr;

        library.reset();
    }

    /**
     * フォントサイズを指定する
     *
     * レンダリングされる文字はこのwidth/heightに収まるように設定される。
     */
    virtual void setSize(const unsigned width, const unsigned height) {
        auto lock = mutex.lock();

        size.x = width;
        size.y = height;
        // 文字サイズは常識の範囲内で行える
        assert(size.x < (4096 * 4));
        assert(size.y < (4096 * 4));

        // ピクセルサイズは多少小さくしておく
        // フォントによってははみ出す場合があるようだ。
        FT_Set_Pixel_Sizes(face, (int) (0.98 * width), (int) (0.98 * height));
//        FT_Set_Pixel_Sizes(face, 0, (int) (0.98 * height));
    }

    /**
     * 指定のバッファに文字をレンダリングし、外径情報を返す。
     *
     * バッファがnullである場合はレンダリングを行わず、外形情報だけを返す。
     * 外形情報がフォントに含まれていない場合、豆腐文字として扱う。
     */
    virtual std::shared_ptr<FontCharactor> rendering(const wchar_t charactor, selection_ptr<IImageDecodeCallback> listener) {
        auto lock = mutex.lock();

        std::shared_ptr<FontCharactorImpl> result;
        int error;
        if ((error = FT_Load_Char(face, charactor, FT_LOAD_RENDER)) != 0) {
            eslog("FT_Load_Char charcode(%x) error(%d)", charactor, error);
            error = FT_Load_Char(face, FT_Get_Char_Index(face, charactor), FT_LOAD_RENDER);
        }

        result.reset(new FontCharactorImpl(this, face, charactor));

        auto lockedListener = listener.lock();
        if (lockedListener) {
            IImageDecodeCallback::ImageInfo info;
            info.format = IImageDecodeCallback::ImageFormat_TrueType;
            info.dstPixelFormat = PixelFormat_R8;
            info.srcPixelFormat = PixelFormat_R8;
            if (error == 0 && face->glyph->bitmap.buffer) {

                // エラーが発生していなければ、外形情報をコピーする
                info.srcWidth = info.dstWidth = result->getBitmapSize().x;
                info.srcHeight = info.dstHeight = result->getBitmapSize().y;
                listener->onImageInfoDecoded(&info);

                // 一括で読み込んだ扱いにする。
                listener->onImageLineDecoded(&info,
                                             unsafe_array<uint8_t>(
                                                     face->glyph->bitmap.buffer,
                                                     info.srcWidth * info.srcHeight),
                                             info.srcHeight
                );
                listener->onImageDecodeFinished(&info, IImageDecodeCallback::ImageDecodeResult_Success);
            } else {
                // エラーが発生していれば、それを通知する
                info.srcWidth = info.dstWidth = 0;
                info.srcHeight = info.dstHeight = 0;
                // エラーが発生した
                listener->onImageDecodeFinished(&info, IImageDecodeCallback::ImageDecodeResult_UnknownError);
            }
        }

        return result;
    }
};

}
}
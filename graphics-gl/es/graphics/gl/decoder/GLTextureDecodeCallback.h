#pragma once

#include "es/protoground.hpp"
#include "es/graphics/image/IImageDecodeCallback.hpp"
#include "es/graphics/gl/engine/GLDevice.h"
#include "es/graphics/gl/resource/GLTexture.h"
#include "es/graphics/factory/GraphicsResourceFactory.h"


namespace es {
namespace gl {

class GLDevice;

/**
 * デコードされたピクセルデータをテクスチャに焼きこむ
 * デコードThreadはDeviceの関係から1Threadに限定する。
 */
class GLTextureDecodeCallback : public virtual Object, public virtual GraphicsResourceFactory::ITextureDecodeCallback {
public:
    /**
     * デコード用のThreadを限定する
     */
    GLTextureDecodeCallback(const GLDevice::query &device);


    /**
     * 読み込み対象のテクスチャを指定する。
     *
     * 指定されなかった場合、適当なテクスチャを生成する。
     */
    void setDecodeTarget(const std::shared_ptr<GLTexture> &texture);

    /**
     * テクスチャに焼きこむ時のXY座標を調整する
     */
    void setUploadOffset(const int x, const int y);


    /**
     * NPOT変換を行わせる場合true
     */
    void setConvertNpotToPot(bool convertNpot);

    /**
     * 画像情報を読み込んだ
     */
    virtual void onImageInfoDecoded(const ImageInfo *info) override;

    /**
     * 画像を指定行読み込んだ
     *
     * 引数lineは使いまわされる可能性があるため、内部的にテクスチャコピー等を行うこと。
     */
    virtual void onImageLineDecoded(const ImageInfo *info, const unsafe_array<uint8_t> pixels, const uint height) override;

    /**
     * 画像のデコードをキャンセルする場合はtrue
     */
    virtual bool isImageDecodeCancel() override;

    /**
     * デコードが完了した
     */
    virtual void onImageDecodeFinished(const ImageInfo *info, const ImageDecodeResult_e result) override;


    /**
     * デコードした成果物を取得する
     */
    virtual std::shared_ptr<ITexture> getTexture() override;

    virtual ~GLTextureDecodeCallback() = default;

private:
    /**
     * 読み込み対象のテクスチャ
     */
    sp<GLTexture> texture;

    GLDevice::query device;

    /**
     * 書き込み先の座標Y
     */
    uint16_t writePixelsY = 0;

    /**
     * NPOTをPOT変換をする場合true
     */
    bool convertNpot = false;

    int16_t offsetX = 0;

    int16_t offsetY = 0;

};


}
}

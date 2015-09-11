#pragma once

#include <es/graphics/resource/FontTextureAtlas.h>
#include "es/protoground.hpp"

#include "es/graphics/resource/ITexture.hpp"
#include "es/graphics/2d/IDisplayTransfer2D.h"
#include "es/graphics/2d/ISpriteRenderingCallback.h"

namespace es {

/**
 * Spriteのレンダリング補助を行う。
 *
 * 実際のレンダリングは別なクラスに任せられる。
 */
class SpriteRenderer : public Object {
public:
    SpriteRenderer();

    void setCallback(const std::shared_ptr<ISpriteRenderingCallback> callback);

    void setDisplay(const std::shared_ptr<IDisplayTransfer2D> display);

    /**
     * フォントレンダリングを行う
     *
     * レイアウトを動的に組み立てるため、多少動作は遅くなる。
     * また、メモリを細かくallocするため断片化の原因となる恐れが有ることに注意すること。
     */
    void renderingText(IDevice *device, const std::string &text, const float x, const float y, const Color color, FontTextureAtlas *atlas) const;

    /**
     * 指定座標に直接レンダリングする
     */
    void renderingImage(const ITexture *texture, const float x, const float y) const;

    /**
     * 指定座標に直接レンダリングする
     */
    void renderingImage(const ITexture *texture, const float x, const float y, const float w, const float h) const;

    /**
     * 特定エリアに描画する
     */
    void renderingImage(const ITexture *texture, const RectI16 &src, const RectI16 &dst, const Color color) const;

    /**
     * 四角形描画を行う
     */
    void renderingRect(const float x, const float y, const float w, const float h, const Color color) const;

    /**
     * 画像を描画する
     */
    void renderingImage(const ITexture *texture, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstW, const float dstH, const float degree, const Color rgba) const;

    /**
     * レンダリングを開始する
     */
    virtual void begin() const;

    /**
     * レンダリングを終了する
     */
    virtual void end() const;

    virtual ~SpriteRenderer() = default;

protected:
    /**
     * レンダリング本体はコールバッククラスに委譲する
     */
    mutable sp<ISpriteRenderingCallback> callback;

    /**
     * ディスプレイ座標変換は別途クラスに委譲する
     */
    sp<IDisplayTransfer2D> display;
private:
    enum Mode_e {
        Mode_Polygon,
        Mode_Font,
    };

    void rendering(const Mode_e mode, const ITexture *texture, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstW, const float dstH, const float degree, const Color rgba) const;

};


}

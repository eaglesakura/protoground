#pragma once

#include <es/system/string/HashStringTable.h>
#include "es/protoground.hpp"
#include "es/system/Bundle.h"
#include "es/graphics/resource/FontTextureAtlas.h"
#include "es/graphics/2d/IDisplayTransfer2D.h"
#include "es/graphics/2d/ISpriteRenderingCallback.h"
#include "es/graphics/image/IImageDecodeCallback.hpp"

namespace es {

class ITexture;

class IMesh;

class IAsset;

class FontTextureAtlas;

class Freetype;

class GraphicsResourceFactory : public virtual Object {
public:
    GraphicsResourceFactory();

    /**
     * 3Dレンダリング用のメッシュを生成する
     */
    virtual std::shared_ptr<IMesh> newMesh(std::shared_ptr<IDevice> device, const Bundle &hint = Bundle()) = 0;

    /**
     * スプライトレンダラを生成する
     */
    virtual std::shared_ptr<SpriteRenderer> newSpriteRenderer(std::shared_ptr<IDevice> device, const Bundle &hint = Bundle());

    /**
     * フォントテクスチャを生成する
     */
    virtual std::shared_ptr<FontTextureAtlas> newFontTextureAtlas(std::shared_ptr<IDevice> device, std::shared_ptr<IAsset> font, const Bundle &hint = Bundle());

    /**
     * テクスチャを読み込む
     *
     * フレームワークが対応しているフォーマットを自動的に認識する。
     * * support : PNG
     * * future  : JPEG
     */
    virtual std::shared_ptr<ITexture> decodeTexture(std::shared_ptr<IDevice> device, std::shared_ptr<IAsset> asset, bool convertPot = false);

    /**
     * テクスチャを読み込む
     *
     * フレームワークが対応しているフォーマットを自動的に認識する。
     * * support : PNG
     * * future  : JPEG
     */
    virtual std::shared_ptr<ITexture> decodeTexture(std::shared_ptr<IDevice> device, std::shared_ptr<IAsset> asset, bool convertPot, PixelFormat_e convertFormat);

    /**
     * テクスチャを読み込む
     *
     * フレームワークが対応しているフォーマットを自動的に認識する。
     * * support : PNG
     * * future  : JPEG
     */
    virtual std::shared_ptr<ITexture> decodeTexture(std::shared_ptr<IDevice> device, std::shared_ptr<IAsset> asset, const Bundle &hint);

    /**
     * コンバートPixelFormatを指定する。
     *
     * デフォルトの場合は変換を行わない。
     */
    static const HashString KEY_TEXTURE_LOAD_PIXELFORMAT_int;

    /**
     * NPOT -> POT変換を行う場合はtrue。
     *
     * デフォルトは変換を行わない。
     * default = false
     */
    static const HashString KEY_TEXTURE_LOAD_POT_CONVERT_bool;

    /**
     * 一度のデコードで読み込む行数
     *
     * デフォルトでは8行(縦8pix)読み込む。
     * 非対応のデコーダーの場合は一括で読み込み、それを分割してコールバックする。
     */
    static const HashString KEY_TEXTURE_LOAD_READLINE_ONCE_int;

    class ITextureDecodeCallback;

    /**
     * Engineの実装依存コード
     */
protected:

    sp<Freetype> freetype;

    /**
     * フォント用のテクスチャアロケータを生成する
     */
    virtual std::shared_ptr<FontTextureAtlas::TextureAllocator> newFontTextureAllocator(std::shared_ptr<IDevice> device, const Bundle &hint) = 0;

    /**
     * ディスプレイ座標変換機を生成する
     *
     * for SpriteRenderer
     */
    virtual std::shared_ptr<IDisplayTransfer2D> newDisplayTransfer2D(std::shared_ptr<IDevice> device, const Bundle &hint) = 0;

    /**
     * スプライトレンダリングクラスを生成する
     *
     * for SpriteRenderer
     */
    virtual std::shared_ptr<ISpriteRenderingCallback> newSpriteRenderingCallback(std::shared_ptr<IDevice> device, const Bundle &hint) = 0;

    /**
     * 画像デコーダーを取得する
     */
    virtual std::shared_ptr<ITextureDecodeCallback> newImageDecodeListener(std::shared_ptr<IDevice> device, const Bundle &hint) = 0;

public:
    /**
     * テクスチャ読み込みを行う拡張Callback
     *
     * 読み込み後のテクスチャが取得できるようにする。
     */
    class ITextureDecodeCallback : public virtual IImageDecodeCallback {
    public:
        /**
         * デコード完了したテクスチャを取得する
         */
        virtual std::shared_ptr<ITexture> getTexture() = 0;
    };
};

}
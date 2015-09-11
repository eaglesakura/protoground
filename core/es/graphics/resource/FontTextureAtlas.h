#pragma once

#include "es/protoground.hpp"
#include "es/math/Rect.hpp"
#include "es/graphics/font/FontFace.h"
#include "es/memory/SafeArray.hpp"
#include <vector>
#include <map>
#include <es/system/Hash64.h>

namespace es {

class ITexture;

class IDevice;

class FontTextureAtlas : public Object {
public:
    FontTextureAtlas(const std::shared_ptr<FontFace> font);

    virtual ~FontTextureAtlas() = default;

    class FontArea;

    class TextureAllocator;

    /**
     * テキストを焼きこむ
     *
     * 新規に焼きこまれたテクスチャ数を返却する
     * bake中にgetTextures()するとiteratorが無効になる可能性があるため、外部でlockすることを推奨する。
     */
    uint bake(IDevice *device, const std::wstring text);

    /**
     * 管理しているテクスチャリストを取得する
     */
    const std::vector<std::shared_ptr<ITexture> > &getTextures() const;

    /**
     * 焼きこまれている文字数
     */
    uint32_t getBakeCharacters() const;

    /**
     * 焼きこみ対象テクスチャのアロケータを登録する
     */
    void setAllocator(std::shared_ptr<TextureAllocator> allocator);

    /**
     * 管理しているアトラスを解放する
     */
    void clear();

    /**
     * エリアをピックアップする。
     * bakeされていない場合、nullptrを返す。
     */
    const std::shared_ptr<FontArea> pick(const wchar_t charactor) const;

    const std::shared_ptr<FontFace> &getFont() const { return font; }

    class FontArea {
    public:
        uint16_t getIndex() const;

        std::shared_ptr<FontCharactor> getCharactor() const;

        const RectI16 &getArea() const;

    private:
        /**
         * テクスチャのインデックス
         */
        uint16_t index;

        /**
         * 字体情報
         */
        std::shared_ptr<FontCharactor> charactor;

        /**
         * フォントBitmapが格納されている位置
         * ピクセル単位座標
         */
        RectI16 area;

        friend class FontTextureAtlas;
    };

    class TextureAllocator {
    public:
        /**
         * 新規にテクスチャを生成させる
         *
         * テクスチャは最低でもフォントの大きさ以上の解像度を持つ必要がある。
         * テクスチャの内部形式は1画素1byte単色(R8、A8等）である必要がある。
         */
        virtual std::shared_ptr<ITexture> newTexture(IDevice *device) = 0;

        /**
         * 画素情報のアップロードを行わせる
         * メソッド呼び出し時点でTextureはバインド済みであるため、アップロードのみを行うこと。
         */
        virtual void upload(IDevice *device, ITexture *texture, const uint offsetX, const uint offsetY, const uint width, const uint height, const unsafe_array<uint8_t> &buffer) = 0;

        virtual ~TextureAllocator() = default;
    };

private:
    /**
     * 管理しているテクスチャ一覧
     */
    std::vector<sp<ITexture> > textures;

    struct {
        /**
         * 次にテクスチャを焼き付けるX座標
         */
        uint16_t x = 0;

        /**
         * 次にテクスチャを焼き付けるY座標
         */
        uint16_t y = 0;

        /**
         * 現在の焼き込みラインの一番の高さ
         */
        uint16_t lineHeight = 0;
    } current;

    /**
     * 座標マップ
     */
    std::map<Hash64, sp<FontArea> > atlasMap;

    /**
     * テクスチャ生成クラス
     */
    sp<TextureAllocator> allocator;

    /**
     * フォント情報
     */
    sp<FontFace> font;
};


}

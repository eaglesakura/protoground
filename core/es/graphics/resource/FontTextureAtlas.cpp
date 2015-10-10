#include "es/internal/protoground-internal.hpp"
#include "FontTextureAtlas.h"
#include "es/graphics/font/FontCharactor.h"
#include "es/util/MapUtil.hpp"

#include "es/graphics/resource/ITexture.hpp"

namespace es {

namespace {
Hash64 make_hash(wide_char ch, const uint16_t sizeX, const uint16_t sizeY) {
    return Hash64::from(((uint64_t) ch) << 32 | ((uint64_t) sizeX << 16) | ((uint64_t) sizeY));
}
}


FontTextureAtlas::FontTextureAtlas(const std::shared_ptr<FontFace> newFont) : font(newFont) {
    assert(font);
}

unsigned FontTextureAtlas::bake(IDevice *device, const wide_string &text) {
    class TempListener : public IImageDecodeCallback {
    public:
        ImageInfo info;
        unsafe_array<uint8_t> pixels;
        ImageDecodeResult_e result = ImageDecodeResult_UnknownError;

        virtual void onImageInfoDecoded(const ImageInfo *info) {
            this->info = *info;
        }

        /**
         * 画像を指定行読み込んだ
         *
         * 引数lineは使いまわされる可能性があるため、内部的にテクスチャコピー等を行うこと。
         */
        virtual void onImageLineDecoded(const ImageInfo *info, const unsafe_array<uint8_t> pixels, const unsigned height) {
            this->pixels = pixels;
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
            this->result = result;
        }
    } tempListener;

    if (text.empty()) {
        return 0;
    }

    // 新規にAtlasを生成する
    if (textures.empty()) {
        auto texture = allocator->newTexture(device);
        textures.push_back(texture);
    }

    const auto size = font->getSize();
    unsigned result = 0;
    for (const wide_char code : text) {
        Hash64 hash = make_hash(code, size.x, size.y);
        if (atlasMap.find(hash) != atlasMap.end()) {
            // 既にatlasを保持している
            continue;
        }

        auto texture = textures[textures.size() - 1];
        auto charactor = font->rendering(code, &tempListener);
        if (charactor && tempListener.result == IImageDecodeCallback::ImageDecodeResult_Success) {
            // レンダリングに成功したので、テクスチャに焼きこむ
            i16vec2 bitmapSize = charactor->getBitmapSize();
            if ((current.x + bitmapSize.x + 1) > texture->getWidth()) {
                // この行をはみ出す場合は改行する
                current.y += (current.lineHeight + 1);
                current.x = 0;
                current.lineHeight = 0;
            }
            if ((current.y + bitmapSize.y) > texture->getHeight()) {
                // テクスチャからはみ出る場合はテクスチャも生成する
                textures.push_back(allocator->newTexture(device));
                texture = textures[textures.size() - 1];
                assert(texture);
                current.y = 0;
            }

            sp<FontArea> area(new FontArea());
            area->area.left = current.x;
            area->area.top = current.y;
            area->area.right = area->area.left + tempListener.info.dstWidth;
            area->area.bottom = area->area.top + tempListener.info.dstHeight;
            area->charactor = charactor;
            area->index = textures.size() - 1;

            // 焼きこみを行う
            allocator->upload(device,
                              texture.get(),
                              area->area.left, area->area.top,
                              area->area.width(), area->area.height(),
                              tempListener.pixels
            );

            // 次のテクスチャ座標を設定
            current.x += (area->area.width() + 1);
            current.lineHeight = std::max<int16_t>(area->area.height() + 1, current.lineHeight);

            // アトラス情報を保存
            atlasMap.insert(std::make_pair(hash, area));
        }
        ++result;
    }

    return result;
}

const std::vector<std::shared_ptr<es::ITexture> > &FontTextureAtlas::getTextures() const {
    return textures;
}

void FontTextureAtlas::setAllocator(std::shared_ptr<TextureAllocator> allocator) {
    this->allocator = allocator;
}

const std::shared_ptr<FontTextureAtlas::FontArea> FontTextureAtlas::pick(const wide_char charactor) const {
    auto size = font->getSize();
    return util::find(atlasMap, make_hash(charactor, size.x, size.y));
}

uint16_t FontTextureAtlas::FontArea::getIndex() const {
    return index;
}

std::shared_ptr<FontCharactor> FontTextureAtlas::FontArea::getCharactor() const {
    return charactor;
}

const RectI16 &FontTextureAtlas::FontArea::getArea() const {
    return area;
}

void FontTextureAtlas::clear() {
    textures.clear();
    atlasMap.clear();
}

uint32_t FontTextureAtlas::getBakeCharacters() const {
    return (uint32_t) atlasMap.size();
}
}

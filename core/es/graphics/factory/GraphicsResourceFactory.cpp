#include "GraphicsResourceFactory.h"
#include <es/graphics/image/png/PngFileDecoder.h>
#include "es/internal/protoground-internal.hpp"
#include "es/graphics/2d/SpriteRenderer.h"
#include "es/graphics/font/Freetype.h"

namespace es {

/**
 * コンバートPixelFormatを指定する。
 *
 * デフォルトの場合は変換を行わない。
 */
const HashString GraphicsResourceFactory::KEY_TEXTURE_LOAD_PIXELFORMAT_int = ES_GLOBAL_HASH("Graphics.Texture.PixelFormat");

/**
 * NPOT -> POT変換を行う場合はtrue。
 *
 * デフォルトは変換を行わない。
 */
const HashString GraphicsResourceFactory::KEY_TEXTURE_LOAD_POT_CONVERT_bool = ES_GLOBAL_HASH("Graphics.Texture.PotConvert");

/**
 * 一度のRead処理で読み込むピクセル高
 */
const HashString GraphicsResourceFactory::KEY_TEXTURE_LOAD_READLINE_ONCE_int = ES_GLOBAL_HASH("Graphics.Texture.ReadOnceLine");

GraphicsResourceFactory::GraphicsResourceFactory() {

}

std::shared_ptr<SpriteRenderer> GraphicsResourceFactory::newSpriteRenderer(std::shared_ptr<IDevice> device, const Bundle &hint) {
    sp<SpriteRenderer> result;
    result.reset(new SpriteRenderer());
    result->setCallback(this->newSpriteRenderingCallback(device, hint));
    result->setDisplay(this->newDisplayTransfer2D(device, hint));

    return result;
}

std::shared_ptr<ITexture> GraphicsResourceFactory::decodeTexture(std::shared_ptr<IDevice> device, std::shared_ptr<IAsset> asset, bool convertPot) {
    Bundle hint;
    hint.putBool(GraphicsResourceFactory::KEY_TEXTURE_LOAD_POT_CONVERT_bool, convertPot);
    return decodeTexture(device, asset, hint);
}

std::shared_ptr<ITexture> GraphicsResourceFactory::decodeTexture(std::shared_ptr<IDevice> device, std::shared_ptr<IAsset> asset, bool convertPot, PixelFormat_e convertFormat) {
    Bundle hint;
    hint.putBool(GraphicsResourceFactory::KEY_TEXTURE_LOAD_POT_CONVERT_bool, convertPot);
    hint.putInt32(GraphicsResourceFactory::KEY_TEXTURE_LOAD_PIXELFORMAT_int, (int32_t) convertFormat);
    return decodeTexture(device, asset, hint);
}

std::shared_ptr<ITexture> GraphicsResourceFactory::decodeTexture(std::shared_ptr<IDevice> device, std::shared_ptr<IAsset> asset, const Bundle &hint) {
    assert((bool) device);
    if (!((bool) asset)) {
        // アセットが読み込めていない
        return sp<ITexture>();
    }

    auto listener = this->newImageDecodeListener(device, hint);
    // 先頭領域をチェックする
    auto header = asset->read(128);

    if (PngFileDecoder::isPngFile(header)) {
        // is PNG
        PngFileDecoder decoder;
        const int pixelFormat = hint.getInt32(GraphicsResourceFactory::KEY_TEXTURE_LOAD_PIXELFORMAT_int, -1);
        if (pixelFormat >= 0) {
            decoder.setConvertPixelFormat((PixelFormat_e) pixelFormat);
        }
        decoder.setConvertPot(hint.getBool(GraphicsResourceFactory::KEY_TEXTURE_LOAD_POT_CONVERT_bool, false));
        decoder.setReadedBuffer(header);
        decoder.setOnceReadHeight(hint.getInt32(GraphicsResourceFactory::KEY_TEXTURE_LOAD_READLINE_ONCE_int, 8));
        decoder.load(asset, listener);
    }
    return listener->getTexture();
}

std::shared_ptr<FontTextureAtlas> GraphicsResourceFactory::newFontTextureAtlas(std::shared_ptr<IDevice> device, std::shared_ptr<IAsset> font, const Bundle &hint) {
    if (!freetype) {
        freetype.reset(new Freetype());
    }

    auto face = freetype->load(util::toOnMemory(font));
    assert(face);

    sp<FontTextureAtlas> result(new FontTextureAtlas(face));
    result->setAllocator(this->newFontTextureAllocator(device, hint));
    return result;
}

}
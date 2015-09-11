#include "SpriteRenderer.h"
#include "es/math/protoground-glm.hpp"
#include <es/system/process/IProcessContext.h>
#include <es/graphics/font/TextLayoutManager.h>
#include <es/internal/log/Log.h>
#include "es/system/string/IStringConverter.h"
#include "es/graphics/engine/IDevice.hpp"
#include "es/graphics/engine/IRenderingSurface.hpp"


namespace es {


SpriteRenderer::SpriteRenderer() {
    
}

void SpriteRenderer::setCallback(const std::shared_ptr<ISpriteRenderingCallback> callback) {
    assert(callback);
    
    this->callback = callback;
}

void SpriteRenderer::setDisplay(const std::shared_ptr<IDisplayTransfer2D> display) {
    assert(display);
    
    this->display = display;
}

void SpriteRenderer::renderingImage(const ITexture *texture, const float x, const float y) const {
    assert(texture);
    rendering(Mode_Polygon, texture, 0, 0, texture->getWidth(), texture->getHeight(), x, y, texture->getWidth(), texture->getHeight(), 0, Color::white());
}

void SpriteRenderer::renderingImage(const ITexture *texture, const float x, const float y, const float w, const float h) const {
    assert(texture);
    rendering(Mode_Polygon, texture, 0, 0, texture->getWidth(), texture->getHeight(), x, y, w, h, 0, Color::white());
}

void SpriteRenderer::renderingImage(const ITexture *texture, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstW, const float dstH, const float degree, const Color rgba) const {
    rendering(Mode_Polygon, texture, srcX, srcY, srcW, srcH, dstX, dstY, dstW, dstH, degree, rgba);
}


void SpriteRenderer::renderingText(IDevice *device, const std::string &text, const float x, const float y, const Color color, FontTextureAtlas *atlas) const {
    assert(device);
    assert(device->getSurface());

    const auto wideString = IProcessContext::getInstance()->getStringConverter()->toWideString(text);

    const wide_char DOT = L"."[0];
    const wide_char TOFU = L"□"[0];
    const wide_char SPACE = L" "[0];
    const wide_char ZEN_SPACE = L"　"[0];

    // テクスチャの焼きこみを行う
    {
        // 末尾"..."分も焼きこむ
        atlas->bake(device, wideString);
        atlas->bake(device, std::wstring(L"."));
        atlas->bake(device, std::wstring(L"□"));
    }


    // 画面全体にレイアウトする
    TextLayoutManager layout(atlas->getFont());
    {
        layout.setLayoutSize(device->getSurface()->getWidth(), device->getSurface()->getHeight());

        auto dotFace = atlas->pick(DOT)->getCharactor();
        if (dotFace) {
            std::vector<sp<FontCharactor> > footer = {dotFace, dotFace, dotFace};
            layout.setFooderText(footer);
        }

        for (const wide_char &c : wideString) {
            if (c == SPACE) {
                // 半角スペースを開ける
                layout.addSpace(atlas->getFont()->getSize().x / 3);
            } else if (c == ZEN_SPACE) {
                // 全角スペースを開ける
                layout.addSpace(atlas->getFont()->getSize().x);
            } else {
                auto once = atlas->pick(c);
                if (!once) {
                    once = atlas->pick(TOFU);
                }
                assert(once);
                layout.add(once->getCharactor());
            }
        }
    }

    // レイアウト結果を描画する
    auto layoutText = layout.getText();
    auto textures = atlas->getTextures();
    for (auto item : layoutText) {
        auto face = atlas->pick(item->getCharactor()->getCode());
        auto tex = textures[face->getIndex()];

        const RectI16 &src = face->getArea();
        const RectI16 &dst = item->getBitmapArea();
        rendering(Mode_Font, tex.get(), src.left, src.top, src.width(), src.height(), x + dst.left, y + dst.top, dst.width(), dst.height(), 0, color);
    }
}

void SpriteRenderer::renderingImage(const ITexture *texture, const RectI16 &src, const RectI16 &dst, const Color color) const {
    renderingImage(texture, src.left, src.top, src.width(), src.height(), dst.left, dst.top, dst.width(), dst.height(), 0, color);
}

void SpriteRenderer::renderingRect(const float x, const float y, const float w, const float h, const Color rgba) const {
    renderingImage(nullptr, 0, 0, 0, 0, x, y, w, h, 0, rgba);
}

void SpriteRenderer::begin() const {
    assert(callback);
    assert(display);
    
    callback->beginRendering(const_cast<SpriteRenderer *>(this));
}

void SpriteRenderer::end() const {
    callback->endRendering(const_cast<SpriteRenderer *>(this));
}

void SpriteRenderer::rendering(const SpriteRenderer::Mode_e mode, const ITexture *texture, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstW, const float dstH, const float degree, const Color rgba) const {
    if (es::equals(dstW, 0.0f) || es::equals(dstH, 0.0f)) {
        // 幅か高さが0であれば、レンダリングの必要はない
        return;
    }

    ISpriteRenderingCallback::RenderingState state;
    ISpriteRenderingCallback::RenderingQuadInstance instance;


    if (mode == Mode_Font) {
        state.mode = ISpriteRenderingCallback::RenderingMode_Text;
    } else {
        state.mode = ISpriteRenderingCallback::RenderingMode_QuadFill;
    }
    instance.rotateRadian = glm::radians(degree);
    instance.texture = const_cast<ITexture *>(texture);
    instance.color = rgba;

    // coord計算
    if (texture) {
        RectI16 area;
        i16vec2 size((int16_t) texture->getWidth(), (int16_t) texture->getHeight());
        texture->getImageArea(&area);

        RectF &srcCoord = instance.srcCoord;
        srcCoord.left = ((float) area.left + srcX) / (float) size.x;
        srcCoord.top = ((float) area.top + srcY) / (float) size.y;
        srcCoord.right = srcCoord.left + (srcW / (float) size.x);
        srcCoord.bottom = srcCoord.top + (srcH / (float) size.y);
    }

    // デバイス座標に変換
    {
        RectF &dstQuad = instance.dstQuad;
        const vec2 leftTop = display->getSpritePositionToDevice(dstX, dstY);
        const vec2 rightBottom = display->getSpritePositionToDevice(dstX + dstW, dstY + dstH);
        state.surfaceAspect = display->getSpriteSurfaceAspect();
        dstQuad.left = leftTop.x;
        dstQuad.top = leftTop.y;
        dstQuad.right = rightBottom.x;
        dstQuad.bottom = rightBottom.y;
    }

    callback->requestRendering(const_cast<SpriteRenderer *>(this), &state, 1, &instance);
}

}

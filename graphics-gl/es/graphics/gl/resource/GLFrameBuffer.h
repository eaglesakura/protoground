#pragma once

#include "es/protoground.hpp"
#include "es/OpenGL.hpp"
#include <es/graphics/engine/IGraphicsResource.h>
#include <es/graphics/gl/engine/GLDevice.h>
#include "es/math/protoground-glm.hpp"
#include "GLTexture.h"

namespace es {
namespace gl {

/**
 * レンダリング用のフレームバッファを管理する
 */
class GLFrameBuffer : public IGraphicsResource {
public:
    GLFrameBuffer(unsigned width, unsigned height);

    unsigned getWidth() const;

    unsigned getHeight() const;

    /**
     * 深度・ステンシルバッファを生成する
     */
    void createDepthStencilBuffer(GLDevice *device, unsigned depthBits, unsigned stencilBits);

    /**
     * カラーテクスチャを用意する
     */
    std::shared_ptr<GLTexture> createColorBuffer(GLDevice *device, const PixelFormat_e format);

    /**
     * カラーテクスチャ数を取得する
     */
    uint32_t getTextureCount() const;

    const std::shared_ptr<GLTexture> &getTexture(const unsigned index) const;

    /**
     * フレームバッファをバインドする
     * 古いフレームバッファを保持し、unbind時に元に戻す。
     */
    void bind();

    /**
     * フレームバッファをアンバインドする。
     * 古いフレームバッファをバインドし直す。
     */
    void unbind();

    /**
     * フレームバッファの正当性チェックを行う
     */
    void checkFramebufferStatus() const;

    virtual ~GLFrameBuffer();

    virtual void dispose() override;

private:
    i16vec2 size;

    /**
     *
     */
    GLuint handle = 0;

    /**
     * バインド時に保持しておく古いハンドル
     */
    GLuint oldHandle = 0;

    /**
     * バインドされたカラーテクスチャ
     */
    std::vector<sp<GLTexture> > colors;

    struct RenderBuffer {
        RenderBuffer();

        ~RenderBuffer();

        void resize(GLenum internalFormat, unsigned width, unsigned height);

        GLuint handle = 0;
    };

    /**
     * 深度・ステンシルバッファ
     */
    sp<RenderBuffer> depth;

    /**
     * ステンシルテクスチャ
     */
    sp<RenderBuffer> stencil;
};

}
}

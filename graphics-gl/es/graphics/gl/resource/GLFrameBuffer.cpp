#include "GLFrameBuffer.h"
#include <es/internal/log/Log.h>
#include "es/graphics/gl/engine/GLDevice.h"
#include "es/graphics/gl/engine/GLGPUCapacity.h"

namespace es {
namespace gl {


GLFrameBuffer::GLFrameBuffer(unsigned width, unsigned height) {
    glGenFramebuffers(1, &handle);

    assert(handle);
    assert_gl();

    size.x = (int16_t) width;
    size.y = (int16_t) height;
}

GLFrameBuffer::~GLFrameBuffer() {
    this->dispose();
}

void GLFrameBuffer::dispose() {
    if (handle) {
        glDeleteFramebuffers(1, &handle);
        handle = 0;
        assert_gl();
    }

    colors.clear();
    depth.reset();
    stencil.reset();
}

void GLFrameBuffer::bind() {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &oldHandle);
    assert_gl();

    glBindFramebuffer(GL_FRAMEBUFFER, handle);
    assert_gl();


    // レンダリングに利用するバッファテーブルを更新する
    if (colors.size() > 1) {
        constexpr GLenum targetBuffers[] = {
                GL_COLOR_ATTACHMENT0,
                GL_COLOR_ATTACHMENT1,
                GL_COLOR_ATTACHMENT2,
                GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4,
                GL_COLOR_ATTACHMENT5,
                GL_COLOR_ATTACHMENT6,
                GL_COLOR_ATTACHMENT7,
        };
        glDrawBuffers((GLsizei) colors.size(), targetBuffers);
        assert_gl();
    }
}

void GLFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, oldHandle);
    oldHandle = 0;
    assert_gl();
}

std::shared_ptr<GLTexture> GLFrameBuffer::createColorBuffer(GLDevice *device, const PixelFormat_e format) {
    // カラー用テクスチャを生成する
    sp<GLTexture> tex(new GLTexture());
    tex->bind(device->getTextureState());
    tex->allocPixelMemory(format, 0, size.x, size.y);
    tex->setFilter(GL_NEAREST, GL_NEAREST);
    tex->setWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    tex->unbind(device->getTextureState());

    // アタッチメントを登録する
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0 + colors.size(),
                           GL_TEXTURE_2D,
                           tex->getTextureHandle(),
                           0);
    assert_gl();

    // バッファを保持する
    colors.push_back(tex);
    return tex;
}

void GLFrameBuffer::createDepthStencilBuffer(GLDevice *device, unsigned depthBits, unsigned stencilBits) {
    GLenum internalformat = 0;
    const auto &caps = device->getCapacity();

    if (depthBits >= 32 && caps.isSupport(IGPUCapacity::GPUExtension_Renderbuffer_Depth32)) {
        internalformat = GL_DEPTH_COMPONENT32_OES;
        eslog("alloc depth req(%d) -> D(32bit)", depthBits);
    } else if (depthBits == 24 && stencilBits == 8 && caps.isSupport(IGPUCapacity::GPUExtension_Renderbuffer_PackedDepth24Stencil8)) {
        internalformat = GL_DEPTH24_STENCIL8_OES;
        eslog("alloc depth req(%d) -> D(24bit) S(8bit)", depthBits);
    } else if (depthBits >= 24 && caps.isSupport(IGPUCapacity::GPUExtension_Renderbuffer_Depth24)) {
        internalformat = GL_DEPTH_COMPONENT24_OES;
        eslog("alloc depth req(%d) -> D(24bit)", depthBits);
    } else {
        internalformat = GL_DEPTH_COMPONENT16;
        eslog("alloc depth req(%d) -> D(16bit)", depthBits);
    }

    depth.reset(new RenderBuffer());
    depth->resize(internalformat, size.x, size.y);

    if (internalformat == GL_DEPTH24_STENCIL8_OES) {
        // dpethとステンシルが同一
        stencil = depth;

        // 深度とステンシルを同時に割り当てる
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth->handle);
    } else if (stencilBits == 8) {
        // ステンシルを別個に初期化
        stencil.reset(new RenderBuffer());
        stencil->resize(GL_STENCIL_INDEX8, size.x, size.y);

        // 深度とステンシルを別個に割り当てる
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth->handle);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil->handle);
    } else {
        // 深度だけを割り当てる
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth->handle);
    }
    assert_gl();
}


const std::shared_ptr<GLTexture> &GLFrameBuffer::getTexture(const unsigned index) const {
    assert(index < colors.size());
    return colors[index];
}

GLFrameBuffer::RenderBuffer::RenderBuffer() {
    glGenRenderbuffers(1, &handle);
    assert(handle);
    assert_gl();
}

GLFrameBuffer::RenderBuffer::~RenderBuffer() {
    glDeleteRenderbuffers(1, &handle);
}

void GLFrameBuffer::RenderBuffer::resize(GLenum internalFormat, unsigned width, unsigned height) {
    glBindRenderbuffer(GL_RENDERBUFFER, handle);
    {
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
        assert_gl();
    }
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

unsigned GLFrameBuffer::getWidth() const {
    return size.x;
}

unsigned GLFrameBuffer::getHeight() const {
    return size.y;
}

void GLFrameBuffer::checkFramebufferStatus() const {
#ifdef DEBUG
    GLint check = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (check != GL_FRAMEBUFFER_COMPLETE) {
        eslog("glCheckFramebufferStatus(0x%x)", check);
    }
    assert(check == GL_FRAMEBUFFER_COMPLETE);
#endif
}

uint32_t GLFrameBuffer::getTextureCount() const {
    return colors.size();
}
}
}
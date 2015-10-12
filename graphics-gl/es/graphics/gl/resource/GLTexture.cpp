#include "GLTexture.h"
#include "es/internal/protoground-internal.hpp"

namespace es {
namespace gl {

GLTexture::GLTexture() {
    // gen
    glGenTextures(1, &handle);
    assert(handle);
    assert_gl();
}

GLTexture::~GLTexture() {
    dispose();
}

unsigned GLTexture::getWidth() const {
    return size.tex_width;
}

unsigned GLTexture::getHeight() const {
    return size.tex_height;
}

void GLTexture::setImageSize(unsigned x, unsigned y) {
    assert(x <= getWidth());
    assert(y <= getHeight());

    size.img_width = x;
    size.img_height = y;
}

float GLTexture::getAspect() const {
    return (float) getWidth() / (float) getHeight();
}

bool GLTexture::isPowerOfTwoTexture() {
    return isPowerOfTwo(getWidth()) && isPowerOfTwo(getHeight());
}

void GLTexture::setWrapMode(const GLenum s, const GLenum t) {
    if (s != context.wrapS) {
        glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
        context.wrapS = s;
    }
    if (t != context.wrapT) {
        glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
        context.wrapT = t;
    }
}

void GLTexture::setFilter(const GLenum min, const GLenum mag) {
    if (min != context.minFilter) {
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
        context.minFilter = min;
    }
    if (mag != context.magFilter) {
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
        context.magFilter = mag;
    }
}

void GLTexture::genMipmaps() {
    if (!isPowerOfTwoTexture()) {
        // mipmapが生成できなくても表示上は問題ないため、npotログだけ吐き出して終了
        eslog("texture is non power of two %d x %d", (int) size.tex_width, (int) size.tex_height);
        return;
    }
    glGenerateMipmap(target);
    assert_gl();
}

void GLTexture::setBindTarget(const GLenum target) {
    this->target = target;
}

GLenum GLTexture::getBindTarget() const {
    return this->target;
}

void GLTexture::allocPixelMemory(const PixelFormat_e pixelFormat, const int miplevel, const int width, const int height) {
    if (getWidth() == width && getHeight() == height) {
        // リサイズが不要
        return;
    }

    const GLenum format = Pixel::toGLPixelFormat(pixelFormat);
    const GLenum type = Pixel::toGLPixelDataType(pixelFormat);
    glTexImage2D(GL_TEXTURE_2D, miplevel, format, width, height, 0, format, type, NULL);
    size.img_width = size.tex_width = width;
    size.img_height = size.tex_height = height;
    allocated = true;
}


void GLTexture::onAllocated() {
    allocated = true;
}


bool GLTexture::isAllocated() const {
    return allocated;
}

unsigned GLTexture::bind(std::shared_ptr<GLTextureState> state) {
    int index = state->getBindedTextureUnitIndex(target, handle);
    if (index >= 0) {
        // 既にバインド済みのため、Activeのみを切り替える
        state->activeTexture(index);
        return index;
    } else {
        // 空いているユニットから拝借する。
        index = state->getFreeTextureUnitIndex(true);
        assert(index >= 0);

        // 指定したIndexにバインドされていないため、activeとbindの両方を切り替える
        state->activeTexture(index);
        state->bindTexture(target, handle);

        return (unsigned) index;
    }
}

void GLTexture::bind(const unsigned index, std::shared_ptr<GLTextureState> state) {
    if (state->isBindedTexture(index, target, handle)) {
        // 指定したIndexに既にバインドされていたら、activeだけを切り替えて何もしない
        state->activeTexture(index);
        return;
    } else {
        // 指定したIndexにバインドされていないため、activeとbindの両方を切り替える
        state->activeTexture(index);
        state->bindTexture(target, handle);
    }
}

void GLTexture::unbind(std::shared_ptr<GLTextureState> state) {
    state->unbindTexture(handle);
}

GLuint GLTexture::getTextureHandle() const {
    return handle;
}

void GLTexture::dispose() {
    if (handle) {
        // 解放する
        glDeleteTextures(1, &handle);
        handle = 0;
    }
}

Object::QueryResult_e GLTexture::queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const {
    PGD_SUPPORT_QUERY(InterfaceId_OpenGL_Texture, GLTexture);
    return Object::queryInterface(interfaceId, resultInterfacePtr);
}

void GLTexture::getImageArea(RectI16 *existArea) const {
    assert(existArea);
    existArea->setXYWH(0, 0, (int16_t) size.img_width, (int16_t) size.img_height);
}

ITexture::Type_e GLTexture::getType() const {
    switch (target) {
#if defined(GL_TEXTURE_1D)
        case GL_TEXTURE_1D:
            return Type_1D;
#endif
#if defined(GL_TEXTURE_EXTERNAL_OES)
        case GL_TEXTURE_EXTERNAL_OES:
            return Type_AndroidExternalImage;
#endif
        case GL_TEXTURE_2D:
            return Type_2D;
        default:
            return Type_Unknown;
    }
}

}
}
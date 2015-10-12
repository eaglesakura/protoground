#pragma once

#include <es/graphics/gl/engine/GLDevice.h>
#include "es/graphics/resource/FontTextureAtlas.h"
#include "es/OpenGL.hpp"
#include "es/graphics/gl/resource/GLTexture.h"

namespace es {
namespace gl {

namespace internal {

class BasicTextureAllocator : public FontTextureAtlas::TextureAllocator {
public:
    BasicTextureAllocator(unsigned newSize) : size(newSize) {
        assert(isPowerOfTwo(newSize));
        assert(newSize >= 2);
    }

    /**
     * 新規にテクスチャを生成させる
     *
     * テクスチャは最低でもフォントの大きさ以上の解像度を持つ必要がある。
     */
    virtual std::shared_ptr<ITexture> newTexture(IDevice *device) override {
        GLDevice::query glDevice(device);

        sp<GLTexture> result(new GLTexture());
        result->bind(glDevice->getTextureState());
        result->setWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        result->setFilter(GL_NEAREST, GL_NEAREST);
        result->allocPixelMemory(PixelFormat_R8, 0, size, size);

        const unsigned BUFFER_DIV = 8;
        const unsigned BUFFER_SIZE = size / BUFFER_DIV;
        std::vector<uint8_t> cache((BUFFER_SIZE) * (BUFFER_SIZE));
        util::zeromemory(&cache);
        for (int i = 0; i < BUFFER_DIV; ++i) {
            for (int k = 0; k < BUFFER_DIV; ++k) {
                // 空のバッファをアップロードしてテクスチャを空白にする
                upload(
                        device,
                        result.get(),
                        BUFFER_SIZE * k, BUFFER_SIZE * i,
                        BUFFER_SIZE, BUFFER_SIZE,
                        unsafe_array<uint8_t>(util::asPointer(cache), cache.size())
                );
            }
        }
        result->unbind(glDevice->getTextureState());
        return result;
    }

    /**
     * 画素情報のアップロードを行わせる
     */
    virtual void upload(IDevice *device,
                        ITexture *image,
                        const unsigned offsetX, const unsigned offsetY,
                        const unsigned width, const unsigned height,
                        const unsafe_array<uint8_t> &buffer) override {
        GLDevice *glDevice = GLDevice::query::from(device);
        GLTexture *texture = GLTexture::query::from(image);
        assert(glDevice && texture);

        texture->bind(glDevice->getTextureState());
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexSubImage2D(
                GL_TEXTURE_2D,
                0,
                offsetX, offsetY,
                width, height,
                Pixel::toGLPixelFormat(PixelFormat_R8),
                Pixel::toGLPixelDataType(PixelFormat_R8),
                (void *) buffer.ptr
        );
        assert_gl();

        texture->unbind(glDevice->getTextureState());
    }

private:
    /**
     * 生成するテクスチャのサイズ
     */
	unsigned size;
};


} /* es::gl::internal */
}
}
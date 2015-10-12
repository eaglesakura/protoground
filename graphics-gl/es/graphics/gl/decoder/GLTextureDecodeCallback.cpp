#include "GLTextureDecodeCallback.h"


namespace es {
namespace gl {


GLTextureDecodeCallback::GLTextureDecodeCallback(const GLDevice::query &device) : device(device) {
}

void GLTextureDecodeCallback::setDecodeTarget(const std::shared_ptr<GLTexture> &texture) {
    this->texture = texture;
}

void GLTextureDecodeCallback::setUploadOffset(const int x, const int y) {
    this->offsetX = x;
    this->offsetY = y;
}

void GLTextureDecodeCallback::setConvertNpotToPot(bool convertNpot) {
    this->convertNpot = convertNpot;
}

void GLTextureDecodeCallback::onImageInfoDecoded(const IImageDecodeCallback::ImageInfo *info) {
    this->writePixelsY = 0;
    if (!texture) {
        texture.reset(new GLTexture());
    }

    assert(device);
    assert(texture);


    texture->bind(device->getTextureState());

    // デフォルトを修正
    texture->setWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture->setFilter(GL_NEAREST, GL_NEAREST);

    // 転送パッキングを修正
    if (Pixel::getPixelBytes(info->dstPixelFormat) == 4) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    } else {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }


    // VRAM領域の確保
    if (!texture->isAllocated()) {
        int allocWidth = info->dstWidth + offsetY;
        int allocHeight = info->dstHeight + offsetY;
        if (convertNpot) {
            allocWidth = toPowerOfTwo(allocWidth);
            allocHeight = toPowerOfTwo(allocHeight);
        }

        texture->allocPixelMemory(info->dstPixelFormat, 0, allocWidth, allocHeight);
        texture->setImageSize(info->dstWidth, info->dstHeight);
        texture->onAllocated();
    }
    assert_gl();
}

void GLTextureDecodeCallback::onImageLineDecoded(const IImageDecodeCallback::ImageInfo *info, const unsafe_array<uint8_t> pixels, const unsigned height) {
    // 部分転送 for 2D
    glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            offsetX, offsetY + writePixelsY,
            info->dstWidth, height,
            Pixel::toGLPixelFormat(info->dstPixelFormat),
            Pixel::toGLPixelDataType(info->dstPixelFormat),
            (void *) pixels.ptr
    );
    writePixelsY += height;
    assert_gl();
}

bool GLTextureDecodeCallback::isImageDecodeCancel() {
    return false;
}

void GLTextureDecodeCallback::onImageDecodeFinished(const IImageDecodeCallback::ImageInfo *info, const IImageDecodeCallback::ImageDecodeResult_e result) {
    glFinish();
    texture->unbind(device->getTextureState());
    assert_gl();
}

std::shared_ptr<ITexture> GLTextureDecodeCallback::getTexture() {
    return texture;
}
}
}
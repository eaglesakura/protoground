#include <es/system/process/IProcessContext.h>
#include <es/graphics/gl/sprite/GLImmediateSpriteRenderer.h>
#include <es/graphics/gl/decoder/GLTextureDecodeCallback.h>
#include "GLResourceFactory.h"
#include "internal/GLBasicTextureAllocator.hpp"
#include "es/graphics/gl/resource/GLMesh.h"

namespace es {
namespace gl {


GLResourceFactory::GLResourceFactory(const std::shared_ptr<AssetManager> assets) :
        assets(assets) {
    assert((bool) this->assets);
}

GLResourceFactory::~GLResourceFactory() {

}

std::shared_ptr<FontTextureAtlas::TextureAllocator> GLResourceFactory::newFontTextureAllocator(std::shared_ptr<IDevice> device, const Bundle &hint) {
    return std::shared_ptr<FontTextureAtlas::TextureAllocator>(
            new internal::BasicTextureAllocator(this->fontTextureAtlasSize)
    );
}

std::shared_ptr<IDisplayTransfer2D> GLResourceFactory::newDisplayTransfer2D(std::shared_ptr<IDevice> device, const Bundle &hint) {
    GLDevice::query glDevice(device);
    return glDevice->getDisplayTransfer2D();
}

std::shared_ptr<ISpriteRenderingCallback> GLResourceFactory::newSpriteRenderingCallback(std::shared_ptr<IDevice> device, const Bundle &hint) {
    // basic factory
//    GLDevice::query glDevice(device);
    sp<GLImmediateSpriteRenderer> result(new GLImmediateSpriteRenderer());
    result->initialize(device, assets);

    return result;
}

std::shared_ptr<GraphicsResourceFactory::ITextureDecodeCallback> GLResourceFactory::newImageDecodeListener(std::shared_ptr<IDevice> device, const Bundle &hint) {
    GLDevice::query glDevice(device);
    sp<GLTextureDecodeCallback> result(new GLTextureDecodeCallback(glDevice));

    result->setConvertNpotToPot(hint.getBool(GraphicsResourceFactory::KEY_TEXTURE_LOAD_POT_CONVERT_bool, false));

    return result;
}

std::shared_ptr<IMesh> GLResourceFactory::newMesh(std::shared_ptr<IDevice> device, const Bundle &hint) {
    return sp<IMesh>(new GLMesh(device));
}
}
}
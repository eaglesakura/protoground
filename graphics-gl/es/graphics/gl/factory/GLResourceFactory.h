#pragma once

#include "es/protoground.hpp"
#include "es/graphics/factory/GraphicsResourceFactory.h"
#include "es/graphics/gl/engine/OpenGLEngine.h"

namespace es {
class AssetManager;
namespace gl {

class GLResourceFactory : public virtual GraphicsResourceFactory {
public:
    GLResourceFactory(const std::shared_ptr<AssetManager> assets);


    virtual std::shared_ptr<IMesh> newMesh(std::shared_ptr<IDevice> device, const Bundle &hint) override;

    virtual std::shared_ptr<FontTextureAtlas::TextureAllocator> newFontTextureAllocator(std::shared_ptr<IDevice> device, const Bundle &hint) override;

    virtual std::shared_ptr<IDisplayTransfer2D> newDisplayTransfer2D(std::shared_ptr<IDevice> device, const Bundle &hint) override;

    virtual std::shared_ptr<ISpriteRenderingCallback> newSpriteRenderingCallback(std::shared_ptr<IDevice> device, const Bundle &hint) override;

    virtual std::shared_ptr<ITextureDecodeCallback> newImageDecodeListener(std::shared_ptr<IDevice> device, const Bundle &hint) override;

    virtual ~GLResourceFactory();

protected:
    sp<AssetManager> assets;

    uint16_t fontTextureAtlasSize = 1024;
};


}
}

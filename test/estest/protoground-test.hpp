#pragma once

#include "es/protoground.hpp"
#include "es/internal/protoground-internal.hpp"
#include "es/asset/IAsset.hpp"
#include "es/asset/IAssetLoader.hpp"
#include "es/asset/AssetManager.h"
#include "es/system/process/IProcessContext.h"
#include "gtest/gtest.h"

#include    "es/graphics/Graphics.hpp"

#ifdef ES_GRAPHICS_OPENGL

#include "es/OpenGL.hpp"
#include "es/graphics/gl/engine/OpenGLEngine.h"
#include "es/graphics/gl/engine/GLDevice.h"

namespace es {
namespace gl {
namespace internal {

class TestingGLEngine : public virtual OpenGLEngine {
protected:

    virtual std::shared_ptr<GLDevice> newDevice(const Hash64 &id) override {
        return std::shared_ptr<GLDevice>(new GLDevice(getCapacity()));
    }

    virtual void releaseDevice(const Hash64 &id, std::shared_ptr<GLDevice> device) override {

    }
};

struct UnitTestEngine {
    sp<OpenGLEngine> engine;
    sp<IDevice> device;
    GLDevice::query glDevice;

    UnitTestEngine() {
        engine.reset(new TestingGLEngine());
        device = engine->getCurrentDevice();
        glDevice.reset(device);
    }

    ~UnitTestEngine() {
        engine->releaseCurrentDevice();
    }
};

}
}
}

#endif

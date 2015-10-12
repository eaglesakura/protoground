#pragma once

#include "es/protoground.hpp"
#include "es/graphics/gl/engine/OpenGLEngine.h"
#include "es/glfw/device/GlfwDevice.h"
#include <map>

namespace es {

class GlfwEngine : public gl::OpenGLEngine {
public:
    GlfwEngine(const Hash64 &windowThreadId, const std::shared_ptr<GlfwDevice> &windowDevice);

protected:
    virtual std::shared_ptr<gl::GLDevice> newDevice(const Hash64 &id) override;

    virtual void releaseDevice(const Hash64 &id, std::shared_ptr<gl::GLDevice> device) override;

private:
    struct DeviceData;
    Hash64 windowThreadId;
    sp<GlfwDevice> windowDevice;
    std::map<Hash64, sp<DeviceData> > deviceMap;
};

}



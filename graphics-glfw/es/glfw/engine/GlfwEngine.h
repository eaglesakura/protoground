#pragma once

#include "es/protoground.hpp"
#include "es/graphics/gl/engine/OpenGLEngine.h"
#include "es/glfw/device/GlfwDevice.h"
#include <map>
#include <list>
#include "es/system/Thread.hpp"

namespace es {

class GlfwEngine : public gl::OpenGLEngine {
public:
    GlfwEngine(const Hash64 &windowThreadId, const std::shared_ptr<GlfwDevice> &windowDevice);

    void    reserveOffscreenDevices(unsigned num);
protected:
    virtual std::shared_ptr<gl::GLDevice> newDevice(const Hash64 &id) override;

    virtual void releaseDevice(const Hash64 &id, std::shared_ptr<gl::GLDevice> device) override;

private:
    struct DeviceData;
    Hash64 windowThreadId;
    sp<GlfwDevice> windowDevice;
    std::map<Hash64, sp<DeviceData> > deviceMap;

    // 事前確保しておいたオフスクリーンデバイス
    std::list< sp<GlfwDevice> >  rsvOffscreenDevices;
    // 
    es::mutex   deviceMutex;
};

}



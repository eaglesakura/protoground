#include "GlfwEngine.h"
#include "es/graphics/gl/engine/GLDevice.h"
#include "es/internal/protoground-internal.hpp"

namespace es {

namespace gl {

static_assert(sizeof(GLFWglproc) == sizeof(void*), "pointer size error!!");

void *pgdGlfwGetGlProcAddress(void *, char *name) {
    GLFWglproc address = ::glfwGetProcAddress(name);
    if (!address) {
        eslog("GL Proc fail(%s)", name);
    }
    return reinterpret_cast<void *>(reinterpret_cast<uint64_t>(address));
}

}


struct GlfwEngine::DeviceData {
    sp<GlfwDevice> offscreenDevice;
};

GlfwEngine::GlfwEngine(const Hash64 &windowThreadId, const std::shared_ptr<GlfwDevice> &windowDevice) : windowThreadId(windowThreadId), windowDevice(windowDevice) {
}

void GlfwEngine::reserveOffscreenDevices(unsigned num) {
    es::mutex_lock lock(deviceMutex);

    while (rsvOffscreenDevices.size() < num) {
        auto device = GlfwDevice::createOffscreenInstance(1, 1, windowDevice);
        assert(device);

        rsvOffscreenDevices.push_back(device);
    }
}

std::shared_ptr<gl::GLDevice> GlfwEngine::newDevice(const Hash64 &id) {

    if (id != windowThreadId) {
        eslog("Thread(%ull) create offscreen device", id.value());
        // オフスクリーンデバイスの用意
        mutex_lock lock(deviceMutex);

        sp<DeviceData> data(new DeviceData());
        if(rsvOffscreenDevices.empty()) {
            data->offscreenDevice = GlfwDevice::createOffscreenInstance(1, 1, windowDevice);
        } else {
            data->offscreenDevice = rsvOffscreenDevices.front();
            rsvOffscreenDevices.pop_front();
        }
        data->offscreenDevice->bind();

        // マッピング保存
        deviceMap.insert(std::make_pair(id, data));
    } else {
        windowDevice->bind();
    }

    return sp<gl::GLDevice>(new gl::GLDevice(getCapacity()));
}

void GlfwEngine::releaseDevice(const Hash64 &id, std::shared_ptr<gl::GLDevice> device) {
    if (id != windowThreadId) {
        eslog("Thread(%ull) destroy offscreen device", id.value());
        // オフスクリーンデバイスの破棄
        auto data = deviceMap.find(id);
        assert(data != deviceMap.end());

        device->dispose();
        data->second->offscreenDevice->unbind();
        {
            // 予約領域に戻す
            es::mutex_lock lock(deviceMutex);
            rsvOffscreenDevices.push_front(data->second->offscreenDevice);
        }


        deviceMap.erase(data);

    }
    device.reset();
}

}
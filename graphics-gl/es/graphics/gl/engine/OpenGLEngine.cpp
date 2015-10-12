#include "es/internal/protoground-internal.hpp"
#include "OpenGLEngine.h"
#include "GLGPUCapacity.h"
#include "GLDevice.h"
#include "es/graphics/gl/factory/GLResourceFactory.h"
#include <thread>
#include <map>
#include <es/system/process/IProcessContext.h>
#include "es/asset/AssetManager.h"

namespace es {
namespace gl {

class OpenGLEngine::Impl {
public:
    sp<GLGPUCapacity> capacity;

    /**
     * Threadごとのデバイスマッピング
     *
     * 生成されるDevice数は多くても数個なので、mapでも十分な速度で動作する。
     */
    std::map<std::thread::id, sp<GLDevice> > devices;
};


/**
 * GLFWでの関数ポインタ構築
 */
extern void *pgdGlfwGetGlProcAddress(void *, char *name);

/**
 * EGLでの関数ポインタ構築
 */
extern void *pglEglGetGlProcAddress(void *, char *name);

OpenGLEngine::OpenGLEngine() {

    // デバイスを新規生成
    if (!pgdGlGetCompatVersion()) {
        // 関数のロードを行う
#if defined(BUILD_MacOSX)
        pgdGlCompatInitialize(pgdGlfwGetGlProcAddress, nullptr);
#elif defined(BUILD_Android)
        pgdGlCompatInitialize(pglEglGetGlProcAddress, nullptr);
#endif
        eslog("OpenGL Compat Version(%d)", pgdGlGetCompatVersion());
    }

    impl.reset(new OpenGLEngine::Impl());
    impl->capacity.reset(new GLGPUCapacity());
}

OpenGLEngine::~OpenGLEngine() {

}

std::shared_ptr<IGPUCapacity> OpenGLEngine::getCapacity() {
    return impl->capacity;
}


std::shared_ptr<IDevice> OpenGLEngine::getCurrentDevice() {
    const auto id = std::this_thread::get_id();
    auto itr = impl->devices.find(id);
    if (itr != impl->devices.end()) {
        // デバイスが生成済み
        return itr->second;
    } else {
//        sp<GLDevice> device(new GLDevice(impl->capacity));
        auto device = this->newDevice(Hash64::from(id));
        impl->devices.insert(std::make_pair(id, device));
        return device;
    }
}

void OpenGLEngine::releaseCurrentDevice() {
    const auto id = std::this_thread::get_id();
    auto itr = impl->devices.find(id);
    if (itr != impl->devices.end()) {
        this->releaseDevice(Hash64::from(id), itr->second);
        impl->devices.erase(itr);
    }
}

Object::QueryResult_e OpenGLEngine::queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const {
    return Object::queryInterface(interfaceId, resultInterfacePtr);
}

const IGraphicsEngine::GraphicsEngine_e OpenGLEngine::getEngine() {
    return GraphicsEngine_OpenGL;
}

std::shared_ptr<GraphicsResourceFactory> OpenGLEngine::newGraphicsResourceFactory() {
    auto assets = IProcessContext::getInstance()->getAssetManager();
    return sp<GraphicsResourceFactory>(new GLResourceFactory(assets));
}

}
}
#pragma once

#include "es/protoground.hpp"
#include "es/graphics/engine/IDevice.hpp"
#include "OpenGLEngine.h"
#include "GLGPUCapacity.h"

namespace es {
namespace gl {

class OpenGLEngine;

class GLTextureState;

class GLDevice : public virtual IDevice {
public:

    typedef std::shared_ptr<GLDevice> shared;
    typedef query_ptr<GLDevice, InterfaceId_OpenGL_Device> query;

    enum ResourceFlag_e {
        ResourceFlag_FrameBuffer = 0x1 << 0,
        ResourceFlag_Mesh = 0x1 << 1,
        ResourceFlag_Shader = 0x1 << 2,
        ResourceFlag_All = 0xFFFFFFFF
    };

    GLDevice(sp<IGPUCapacity> caps);


    virtual void syncPlatform() override;

    virtual void clearBuffer(const uint32_t clearFlags) override;

    virtual surface_state getCurrentState() const override;

    virtual void setSurfaceState(const surface_state &state) override;

    virtual void pushSurfaceState(const surface_state &state) override;

    virtual void popSurfaceState() override;

    virtual std::shared_ptr<IDisplayTransfer2D> getDisplayTransfer2D() const override;

    virtual std::shared_ptr<IRenderingSurface> getSurface() const override;

    virtual QueryResult_e queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const override;

    /**
     * GPU能力を取得する
     */
    const GLGPUCapacity &getCapacity() const;

    /**
     * レンダリング状態を変更する
     */
    void setRenderState(const render_state &state);

    /**
     * リソースのバインド解除を行う
     * ResourceFlag_e から複数選択する
     */
    void unbind(const uint32_t resourceFlags);

    /**
     * テクスチャ管理を取得する
     */
    std::shared_ptr<GLTextureState> getTextureState() const;

    virtual void dispose();

    virtual ~GLDevice();

private:
    /**
     * サーフェイス状態を書き換える
     */
    void setSurfaceState(const surface_state &old, const surface_state &state);

    class Impl;

    friend class OpenGLEngine;

    sp<Impl> impl;
};

}
}

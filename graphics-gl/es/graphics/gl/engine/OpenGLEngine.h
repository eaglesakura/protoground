#pragma once

#include "es/protoground.hpp"
#include "es/graphics/engine/IGraphicsEngine.hpp"
#include <es/memory/query_ptr.hpp>
#include <es/graphics/engine/IDevice.hpp>
#include <es/system/Hash64.h>
#include "es/OpenGL.hpp"

namespace es {
namespace gl {

class GLDevice;

class OpenGLEngine : public virtual IGraphicsEngine {
public:
    OpenGLEngine();

    typedef query_ptr<OpenGLEngine, InterfaceId_OpenGL_Engine> query;

    virtual const GraphicsEngine_e getEngine() override;

    virtual std::shared_ptr<IGPUCapacity> getCapacity() override;

    virtual std::shared_ptr<IDevice> getCurrentDevice() override;

    virtual void releaseCurrentDevice() override;

    virtual QueryResult_e queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const override;

    virtual ~OpenGLEngine();

    /**
     * 各種ファクトリを生成させる
     *
     * デフォルトではIProcessContextに紐付いたAssetManagerを生成する。
     * もし更にカスタマイズを行う場合はEngineを継承して動作を上書きする。
     */
    virtual std::shared_ptr<GraphicsResourceFactory> newGraphicsResourceFactory() override;

protected:
    /**
     * デバイスの生成を行わせる
     * 必要であれば、ThreadのEGL初期化を行う
     */
    virtual std::shared_ptr<GLDevice> newDevice(const Hash64 &id) = 0;

    /**
     * デバイスの破棄を行わせる
     * 必要であれば、ThreadのEGLクリーンアップを行う
     */
    virtual void releaseDevice(const Hash64 &id, std::shared_ptr<GLDevice> device) = 0;

private:
    class Impl;

    sp<Impl> impl;
};

}
}



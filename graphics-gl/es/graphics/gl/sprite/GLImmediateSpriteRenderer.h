#pragma once

#include "es/protoground.hpp"
#include "es/OpenGL.hpp"
#include "es/graphics/2d/ISpriteRenderingCallback.h"

#include <es/graphics/gl/engine/GLDevice.h>
#include "es/graphics/gl/engine/GLGPUCapacity.h"
#include "es/graphics/gl/shader/GLVectorUniform.h"
#include "es/graphics/gl/shader/GLColorUniform.h"
#include "es/graphics/gl/shader/GLTextureUniform.h"


namespace es {
class AssetManager;
namespace gl {

class GLQuadPolygon;

class GLShaderProgram;


class GLImmediateSpriteRenderer : public virtual Object, public virtual ISpriteRenderingCallback {
public:
    GLImmediateSpriteRenderer() = default;

    /**
     * 初期化を行う
     *
     * 失敗したらfalseを返す
     */
    virtual bool initialize(sp<IDevice> device, std::shared_ptr<AssetManager> assets);

    /**
     * レンダリングスレッドのContextと関連付ける
     *
     * インスタンスの生成はシェーダーの読み込みが絡む関係上、別なThreadで行われる可能性がある。
     * Threadが移ったら、必ずDeviceを再設定しなければならない。
     */
    void setDevice(GLDevice::query device);

    virtual void beginRendering(SpriteRenderer *sender) override;


    virtual int requestRendering(SpriteRenderer *sender, const RenderingState *state, const uint numInstances, RenderingInstance *instanceArray) override;

    virtual void endRendering(SpriteRenderer *sender) override;

    virtual ~GLImmediateSpriteRenderer() = default;

private:
    GLDevice::query device;

    sp<GLShaderProgram> shader;

    struct {
        /**
         * 回転角
         */
        GLFloatUniform rotate;

        /**
         * アスペクト比
         */
        GLFloatUniform aspect;

        /**
         * ポリゴンデータ用
         */
        GLVector4fUniofrm poly_data;

        /**
         * ポリゴンUV用
         */
        GLVector4fUniofrm poly_uv;

        /**
         * ブレンド色
         */
        GLColorUniform color;

        /**
         * テクスチャ
         */
        GLTextureUniform texture;

        /**
         * 色描画とテクスチャ描画の切り替え
         */
        GLFloatUniform fillMode;
    } uniform;

    sp<GLQuadPolygon> quad;
};


}
}

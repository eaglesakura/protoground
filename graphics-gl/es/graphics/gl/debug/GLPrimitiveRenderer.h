#pragma once

#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"
#include <es/asset/AssetManager.h>
#include <es/graphics/gl/resource/GLShaderProgram.h>
#include <es/graphics/gl/shader/GLVectorUniform.h>
#include <es/graphics/gl/resource/GLMesh.h>
#include "es/graphics/model/VertexAttribute.h"

namespace es {
namespace gl {
namespace debug {

/**
 * デバッグ用のプリミティブをレンダリングする
 */
class GLPrimitiveRenderer : public Object {
public:
    GLPrimitiveRenderer();

    /**
     * 初期化を行う
     */
    void initialize(std::shared_ptr<IDevice> device, const VertexAttribute::Complex &complex, std::shared_ptr<AssetManager> assets);

    /**
     * レンダリング用のカメラを指定する
     */
    void setCamera(const mat4 &look, const mat4 &projection);

    void setCamera(const mat4 &lookProjection);

    /**
     * ワイヤーフレームのレンダリングを行う
     *
     * 頂点はLineListで構築されている必要がある。
     */
    void renderingLines(std::shared_ptr<IDevice> device, const void *lineVertices, const unsigned numLines);


    virtual ~GLPrimitiveRenderer() = default;

private:
    struct {
        GLMatrix4Uniform lookProjection;
    } uniform;

    /**
     * 一度にレンダリングできる最大の線数。
     *
     * それを超えると分割される。
     */
    unsigned renderingMaxLines = 4096;

    /**
     * 視点変換行列
     */
    mat4 lookProjection;

    /**
     * レンダリング用のメッシュ
     */
    sp<GLMesh> mesh;

    /**
     * 頂点の属性情報
     */
    VertexAttribute::Complex complex;

    /**
     * gl/debug/wire
     */
    sp<GLShaderProgram> shader;
};


}

}

}



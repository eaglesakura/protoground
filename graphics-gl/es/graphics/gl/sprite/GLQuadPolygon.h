#pragma once

#include "es/protoground.hpp"
#include "es/OpenGL.hpp"
#include "es/graphics/gl/engine/GLGPUCapacity.h"

namespace es {
class IDevice;
namespace gl {

class GLMesh;

class GLQuadPolygon : public Object {
public:
    struct Vertex;

    GLQuadPolygon() = default;

    /**
     * 頂点情報を更新する。
     * verticesはTRIANGLE_FANで４頂点が必要。
     * verticesがnullptrの場合、デフォルトの頂点情報を利用する。
     */
    void updateVertices(sp<IDevice> device, const Vertex *vertices, const GLint posAttrLocation, const GLint uvAttrLocation);

    void setPrimitiveType(GLenum primitiveType);

    /**
     * レンダリングの開始を宣言する
     */
    void bind();

    /**
     * 描画を行う。
     * レンダリング環境はバインド元に従う。
     */
    void rendering();

    /**
     * 四角形構築用の頂点構造体
     */
    struct Vertex {
        float x;
        float y;

        float u;
        float v;
    };

    /**
     * 解放を行う。
     */
    virtual ~GLQuadPolygon() = default;

private:
    /**
     * レンダリング対象メッシュ
     */
    std::shared_ptr<GLMesh> mesh;

    GLenum primitiveType = GL_TRIANGLE_FAN;
};

}
}



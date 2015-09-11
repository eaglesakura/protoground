#include "GLQuadPolygon.h"
#include "es/memory/SafeArray.hpp"
#include <es/graphics/gl/resource/GLMesh.h>
#include "es/graphics/gl/engine/GLDevice.h"

namespace es {
namespace gl {

void GLQuadPolygon::setPrimitiveType(GLenum primitiveType) {
    this->primitiveType = primitiveType;
}

void GLQuadPolygon::updateVertices(sp<IDevice> device, const GLQuadPolygon::Vertex *vertices, const GLint posAttrLocation, const GLint uvAttrLocation) {
    // 古いメッシュを解放する
    mesh.reset();

    if (posAttrLocation < 0) {
        // 位置属性が無いのでレンダリングできない。
        return;
    }

    const float LEFT = -0.5;
    const float TOP = 0.5;
    const float RIGHT = 0.5;
    const float BOTTOM = -0.5;
    const GLQuadPolygon::Vertex defVertices[] = {
            //
            /**
             // 位置情報
             left, top, //!< 左上
             right, top, //!< 右上
             left, bottom, //!< 左下
             right, bottom, //!< 右下

             //! UV情報
             0, 0, //!< 左上
             1, 0, //!< 右上
             0, 1, //!< 左下
             1, 1, //!< 右下
             */
            // 左上
            {LEFT,  TOP,    0.0f, 0.0f,},
            // 左下
            {LEFT,  BOTTOM, 0.0f, 1.0f},
            // 右下
            {RIGHT, BOTTOM, 1.0f, 1.0f},
            // 右上
            {RIGHT, TOP,    1.0f, 0.0f},
            // end
    };

    const GLQuadPolygon::Vertex *usingVertices = vertices ? vertices : defVertices;

    mesh.reset(new GLMesh(device));
    // 頂点を確保する
    {
        IMesh::AllocOption opt(IMesh::Target_Vertices);
        opt.onceBytes = sizeof(GLQuadPolygon);
        opt.num = 4;
        mesh->alloc(device, opt);

        void *ptr = mesh->lock(device, IMesh::LockOption(IMesh::Target_Vertices, IMesh::LockOption::LockType_WriteOverwrite));
        assert(ptr);

        memcpy(ptr, usingVertices, opt.onceBytes * opt.num);
        mesh->unlock(device, ptr);
    }
    // インデックスを確保する
    {
        IMesh::AllocOption opt(IMesh::Target_Indices);
        opt.onceBytes = 1;
        opt.num = 4;
        mesh->alloc(device, opt);

        auto *ptr = (uint8_t *) mesh->lock(device, IMesh::LockOption(IMesh::Target_Indices, IMesh::LockOption::LockType_WriteOverwrite));
        assert(ptr);

        for (int i = 0; i < 4; ++i) {
            ptr[i] = i;
        }
        mesh->unlock(device, ptr);
    }

    // 属性情報を指定する
    {
        GLMesh::Attribute attr;
        attr.location = posAttrLocation;
        attr.normalize = false;
        attr.offsetHeader = 0;
        attr.size = 2;
        attr.type = GL_FLOAT;
        attr.strideBytes = sizeof(GLQuadPolygon::Vertex);
        mesh->addAttribute(attr);
    }
    {
        GLMesh::Attribute attr;
        attr.location = uvAttrLocation;
        attr.normalize = false;
        attr.offsetHeader = sizeof(float) * 2;
        attr.size = 2;
        attr.type = GL_FLOAT;
        attr.strideBytes = sizeof(GLQuadPolygon::Vertex);
        mesh->addAttribute(attr);
    }

    glFinish();
}

void GLQuadPolygon::bind() {
    if (!mesh) {
        return;
    }
    mesh->bind();
}

void GLQuadPolygon::rendering() {
    if (!mesh) {
        return;
    }
    mesh->rendering(primitiveType);
}

}
}
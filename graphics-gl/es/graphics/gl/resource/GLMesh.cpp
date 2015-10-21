#include "GLMesh.h"
#include <es/graphics/gl/engine/GLDevice.h>
#include <es/memory/Buffer.hpp>
#include "es/internal/protoground-internal.hpp"
#include "es/graphics/model/VertexAttribute.h"

namespace es {
namespace gl {

GLMesh::GLMesh(std::shared_ptr<IDevice> device) {
    GLDevice::query gl(device);
    assert(gl);

    supportArrayObject = gl->getCapacity().isSupport(GLGPUCapacity::GPUExtension_VertexArrayObject);
}

GLMesh::~GLMesh() {
    this->dispose();
}

static uint getMapFlags(const IMesh::LockOption &opt) {
#if defined(__ANDROID__)
    const uint table[] = {
            GL_MAP_READ_BIT, /* LockType_ReadOnly */
            GL_MAP_WRITE_BIT, /* LockType_WriteOnly */
            GL_MAP_WRITE_BIT, /* LockType_WriteOverwrite */
            GL_MAP_WRITE_BIT | GL_MAP_READ_BIT, /* LockType_ReadWrite */
    };
#else
    const uint table[] = {
            GL_MAP_READ_BIT, /* LockType_ReadOnly */
            GL_MAP_WRITE_BIT, /* LockType_WriteOnly */
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT, /* LockType_WriteOverwrite */
            GL_MAP_WRITE_BIT | GL_MAP_READ_BIT, /* LockType_ReadWrite */
    };
#endif
    return table[opt.type];
}

void *GLMesh::lock(std::shared_ptr<IDevice> device, const IMesh::LockOption &option) {
    GLDevice *pDevice = gl::GLDevice::query::from(device.get());
    if (pDevice->getCapacity().isSupport(IGPUCapacity::GPUExtension_VertexArrayObject)) {
        // 既存のArrayBufferから切り離す
        glBindVertexArray(0);
    }

    if (option.target == Target_Vertices) {
        if (vertices.locked) {
            return nullptr;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vertices.handle);
        vertices.locked = glMapBufferRange(GL_ARRAY_BUFFER, option.offsetBytes, (option.bytes ? option.bytes : (vertices.onceBytes * vertices.num)), getMapFlags(option));
        assert_gl();
        return vertices.locked;
    } else if (option.target == Target_Indices) {
        if (indices.locked) {
            return nullptr;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.handle);
        indices.locked = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, option.offsetBytes, (option.bytes ? option.bytes : (indices.onceBytes * indices.num)), getMapFlags(option));
        assert_gl();
        return indices.locked;
    }

    return nullptr;
}

void GLMesh::unlock(std::shared_ptr<IDevice> device, const void *ptr) {
    if (ptr == vertices.locked) {
        vertices.locked = nullptr;
        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else if (ptr == indices.locked) {
        indices.locked = nullptr;
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    }
}

void GLMesh::alloc(std::shared_ptr<IDevice> device, const IMesh::AllocOption &option) {
    uint allocBytes = option.num * option.onceBytes;

    GLenum allocTarget = 0;
    GLuint allocHandle = 0;
    GLenum allocUsage = 0;
    if (option.target == Target_Vertices) {
        if (allocBytes == 0) {
            allocBytes = (vertices.num * vertices.onceBytes);
        }

        if (allocBytes < (vertices.num * vertices.onceBytes)) {
            // 既に必要量を確保済みであれば何もしない
            return;
        }

        if (!vertices.handle) {
            glGenBuffers(1, &vertices.handle);
            assert_gl();
        }
        allocTarget = GL_ARRAY_BUFFER;
        allocHandle = vertices.handle;

        vertices.num = option.num;
        vertices.onceBytes = option.onceBytes;
    } else if (option.target == Target_Indices) {
        if (allocBytes == 0) {
            allocBytes = (indices.num * indices.onceBytes);
        }

        if (allocBytes < (indices.num * indices.onceBytes)) {
            // 既に必要量を確保済みであれば何もしない
            return;
        }

        if (!indices.handle) {
            glGenBuffers(1, &indices.handle);
            assert_gl();
        }
        allocTarget = GL_ELEMENT_ARRAY_BUFFER;
        allocHandle = indices.handle;

        indices.num = option.num;
        indices.onceBytes = option.onceBytes;
    }

    if ((option.flags & AllocOption::Flag_Usage_Dynamic)) {
        allocUsage = GL_DYNAMIC_DRAW;
    } else if ((option.flags & AllocOption::Flag_Usage_Stream)) {
        allocUsage = GL_STREAM_DRAW;
    } else {
        allocUsage = GL_STATIC_DRAW;
    }

    glBindBuffer(allocTarget, allocHandle);
    assert_gl();
    glBufferData(allocTarget, allocBytes, nullptr, allocUsage);
    assert_gl();

    glBindBuffer(allocTarget, 0);
    assert_gl();
}

void GLMesh::addAttribute(const GLMesh::Attribute &attr) {
    if (!attr.valid()) {
        // ロケーションが無効なので、シェーダーで利用されていない。
        return;
    }

    // バッファが生成される前は属性を指定できない
    assert(vertices.handle);

    // attributeをONにする
    if (arrayObject) {
        glBindVertexArray(arrayObject);

        glEnableVertexAttribArray(attr.location);
        assert_gl();

        glVertexAttribPointer(attr.location, attr.size, attr.type, attr.normalize, attr.strideBytes, Buffer::offsetBytes(nullptr, attr.offsetHeader));
        assert_gl();


        glBindVertexArray(0);
    } else {
        glEnableVertexAttribArray(attr.location);
        attributes.push_back(attr);
        assert_gl();
    }
}

void GLMesh::bindArrayObject() {
    assert(!arrayObject);
    if (!supportArrayObject) {
        return;
    }

    glGenVertexArrays(1, &arrayObject);
    assert(arrayObject);
    assert_gl();

    glBindVertexArray(arrayObject);
    if (vertices.handle) {
        glBindBuffer(GL_ARRAY_BUFFER, vertices.handle);
    }
    if (indices.handle) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.handle);
    }
    glBindVertexArray(0);
}

void GLMesh::unbindArrayObject() {
    if (arrayObject) {
        glBindVertexArray(0);
        glDeleteBuffers(1, &arrayObject);
        arrayObject = 0;
        assert_gl();
    }
}

void GLMesh::bind() {
    if (arrayObject) {
        glBindVertexArray(arrayObject);
        assert_gl();
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, vertices.handle);
        assert_gl();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.handle);
        assert_gl();

        for (const Attribute &attr : attributes) {
            if (attr.location < 0) {
                continue;
            }

            // VAOをサポートしないので、手動でキャッシュからバインドを行う
            glVertexAttribPointer(attr.location, attr.size, attr.type, attr.normalize, attr.strideBytes, Buffer::offsetBytes(nullptr, attr.offsetHeader));
            assert_gl();
        }
    }
}

void GLMesh::rendering(const GLenum mode) {
    rendering(mode, 0, indices.num);
}


void GLMesh::rendering(const GLenum mode, const GLsizei offsetIndices, const GLsizei numIndices) {
    GLenum dataType;
    switch (indices.onceBytes) {
        case 1:
            dataType = GL_UNSIGNED_BYTE;
            break;
        case 2:
            dataType = GL_UNSIGNED_SHORT;
            break;
        case 4:
            dataType = GL_UNSIGNED_INT;
            break;
        default:
            assert(false);
    }

    glDrawElements(mode, numIndices, dataType, (GLvoid *) ((size_t) indices.onceBytes * offsetIndices));
    assert_gl();
}

void GLMesh::renderingArray(const GLenum mode, const GLsizei offsetVertices, const GLsizei numVertices) {
    glDrawArrays(mode, offsetVertices, numVertices);
    assert_gl();
}

uint GLMesh::getVertexCount() const {
    return vertices.num;
}

uint GLMesh::getIndexCount() const {
    return indices.num;
}

void GLMesh::dispose() {
    if (vertices.handle) {
        glDeleteBuffers(1, &vertices.handle);
        vertices = GLMesh::Handle();
    }
    if (indices.handle) {
        glDeleteBuffers(1, &indices.handle);
        indices = GLMesh::Handle();
    }

    if (arrayObject) {
        glDeleteVertexArrays(1, &arrayObject);
        arrayObject = 0;
    }
    attributes.clear();
}

Object::QueryResult_e GLMesh::queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const {
    PGD_SUPPORT_QUERY(InterfaceId_OpenGL_Mesh, GLMesh);
    return Object::queryInterface(interfaceId, resultInterfacePtr);
}

bool GLMesh::Attribute::valid() const {
    return location >= 0 && offsetHeader >= 0;
}


GLMesh::Attribute GLMesh::makeAttribute(const VertexAttribute &vertexAttribute, const VertexAttribute::Complex &complex, const GLint location) {
    GLMesh::Attribute attr;
    const VertexAttribute *pVertexAttribute = &vertexAttribute;

    attr.location = location;
    attr.offsetHeader = VertexAttribute::getVertexOffset(vertexAttribute, complex);
    attr.strideBytes = complex.vertexBytes;
    attr.normalize = false;

    if (pVertexAttribute == &VertexAttribute::POSITION_float3) {
        attr.type = GL_FLOAT;
        attr.size = 3;

        // POSは存在しなければならないため、検証を行う
        assert(attr.valid());
    } else if (pVertexAttribute == &VertexAttribute::NORMAL_float3) {
        attr.type = GL_FLOAT;
        attr.size = 3;
    } else if (pVertexAttribute == &VertexAttribute::NORMAL_i4) {
        attr.type = GL_UNSIGNED_SHORT;
        attr.size = 4;
        attr.normalize = true;
    } else if (pVertexAttribute == &VertexAttribute::UV_float2) {
        attr.type = GL_FLOAT;
        attr.size = 2;
    } else if (pVertexAttribute == &VertexAttribute::BONE_INDEX_i4) {
        attr.type = GL_UNSIGNED_SHORT;
        attr.size = 4;
    } else if (pVertexAttribute == &VertexAttribute::BONE_WEIGHT_i4) {
        attr.type = GL_UNSIGNED_BYTE;
        attr.size = 4;
        attr.normalize = true;
    } else if (pVertexAttribute == &VertexAttribute::COLOR_i4) {
        attr.type = GL_UNSIGNED_BYTE;
        attr.size = 4;
        attr.normalize = true;
    } else {
        assert(false);
    }

    return attr;
}
}
}
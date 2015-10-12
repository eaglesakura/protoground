#pragma once

#include "es/protoground.hpp"
#include "es/OpenGL.hpp"
#include <es/graphics/engine/IGraphicsResource.h>

namespace es {
namespace gl {

template<typename T>
class GLUniformBlockData : public IGraphicsResource {
public:
    GLUniformBlockData() {
        glGenBuffers(1, &handle);
        assert_gl();
    }

    virtual ~GLUniformBlockData() {
        this->dispose();
    }

    void bind(GLint blockIndex) const {
        if (!handle || blockIndex < 0) {
            return;
        }
        glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, handle);
    }

    /**
     * データをVRAMへアップロードする
     */
    void upload() const {
        glBindBuffer(GL_UNIFORM_BUFFER, handle);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(T), (void *) &data, GL_DYNAMIC_DRAW);
    }

    GLuint getHandle() const { return handle; }

    virtual void dispose() override {
        if (!handle) {
            return;
        }
        glDeleteBuffers(1, &handle);
        handle = 0;
    }

    /**
     * 直接編集可能なPODデータ
     */
    T data;
private:
    GLuint handle = 0;
};

}
}



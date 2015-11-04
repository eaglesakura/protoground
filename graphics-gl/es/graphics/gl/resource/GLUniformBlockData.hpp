#pragma once

#include "es/protoground.hpp"
#include "es/OpenGL.hpp"
#include <es/graphics/engine/IGraphicsResource.h>
#include "es/graphics/gl/resource/GLShaderProgram.h"

namespace es {
namespace gl {

/**
 * Uniform Blockを管理する
 *
 * Uniform Blockはバッファを利用してUniformの共有をサポートするが、
 * Max Uniformの制限を受けるため、無制限なデータ利用はできないことに注意する。
 */
class GLUniformBlock : public IGraphicsResource {
public:
    GLUniformBlock() {
        glGenBuffers(1, &handle);
        assert_gl();
    }

    virtual ~GLUniformBlock() {
        this->dispose();
    }

    /**
     * シェーダーとの関連付けを行う
     *
     * @param program リンク対象のシェーダ
     * @param shaderLocation シェーダー内でのUniform Blockロケーション
     * @param contextTableIndex EGLContextのテーブルにバインドさせる番号、GL_MAX_UNIFORM_BUFFER_BINDINGS未満の値でなければならない
     */
    void bind(GLShaderProgram *program, GLint shaderLocation, unsigned contextTableIndex) const {
        if (!handle || shaderLocation < 0) {
            return;
        }

        // Contextに対してバインドを行う
        glBindBufferBase(GL_UNIFORM_BUFFER, contextTableIndex, handle);
        assert_gl();

        // Contextのテーブルとシェーダーのロケーションをリンクさせる
        glUniformBlockBinding(program->getProgramHandle(), shaderLocation, contextTableIndex);
    }

    /**
     * データをVRAMへアップロードする
     */
    template<typename T>
    void upload(const T *data) const {
        glBindBuffer(GL_UNIFORM_BUFFER, handle);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(T), (void *) data, GL_DYNAMIC_DRAW);

        assert_gl();
    }

    /**
     * データをVRAMへアップロードする
     */
    void upload(const void *data, const uint32_t bytes) const {
        glBindBuffer(GL_UNIFORM_BUFFER, handle);
        glBufferData(GL_UNIFORM_BUFFER, bytes, data, GL_DYNAMIC_DRAW);

        assert_gl();
    }

    GLuint getHandle() const { return handle; }

    virtual void dispose() override {
        if (!handle) {
            return;
        }
        glDeleteBuffers(1, &handle);
        handle = 0;
    }

private:
    GLuint handle = 0;
};

/**
 * Uniform Blockを管理する
 *
 * UniformBlockはContext上に巨大なテーブルを持っており、glBindBufferBaseで任意のテーブルインデックスにバインドを行う。
 * Context上のテーブルとシェーダー上のテーブルの割当はglUniformBlockBindingで行う。
 */
template<typename T>
class GLUniformBlockData : public GLUniformBlock {
public:
    GLUniformBlockData() = default;

    virtual ~GLUniformBlockData() = default;

    /**
     * データをVRAMへアップロードする
     */
    void upload() const {
        GLUniformBlock::upload <T>(&data);
    }

    /**
     * 直接編集可能なPODデータ
     */
    T data;
};

}
}



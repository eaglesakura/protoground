#pragma once

#include "es/protoground.hpp"
#include "es/OpenGL.hpp"
#include <es/graphics/engine/IGraphicsResource.h>
#include "es/graphics/gl/engine/GLGPUCapacity.h"

namespace es {
namespace gl {

class GLShaderProgram : public IGraphicsResource {
public:
    /**
     * ビルド時に指定されたGLSLバージョンを取得する
     */
    GLGPUCapacity::GLSLVersion_e getGLSLVersion() const;

    /**
     * シェーダープログラムのハンドラを取得する
     */
    GLuint getProgramHandle() const;

    /**
     * プログラム属性変数のロケーションを取得する
     */
    GLint getAttribLocation(const char *name) const;

    /**
     * uniform変数のロケーションを取得する
     */
    GLint getUniformLocation(const char *name) const;

    /**
     * UniformBlockのロケーションを取得する
     */
    GLint getUniformBlockIndex(const char *name) const;

    /**
     * GLと関連付ける
     */
    void bind();

    /**
     * 管理しているリソースの解放を行う
     */
    virtual void dispose() override;

    /**
     * ビルドを行う
     */
    static ::std::shared_ptr<GLShaderProgram> build(const GLGPUCapacity &caps, const std::string &vertex_shader, const std::string &fragment_shader, const GLGPUCapacity::GLSLVersion_e version);

    virtual ~GLShaderProgram();

private:
    GLuint program = 0;
    GLGPUCapacity::GLSLVersion_e version;

    GLShaderProgram(const GLuint program);
};


}
}


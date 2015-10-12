#pragma once

#include    "es/protoground.hpp"
#include    "es/graphics/gl/resource/GLShaderProgram.h"

namespace es {
namespace gl {

class GLShaderProgram;

namespace internal {

struct GLUniformBase {
    /**
     * Uniform値が有効ならばtrueを返す
     */
    bool valid() const;

    /**
     * ロケーション設定を行う
     */
    void setLocation(const GLint location);

    /**
     * uniform locationを取得する
     */
    bool setLocation(const std::shared_ptr<GLShaderProgram> program, const char *name);

    /**
     * Uniformロケーションを取得する。
     * 無効な場合、UNIFORM_DISABLE_INDEXが帰る.
     */
    GLint getLocation() const;

protected:
    GLint location = -1;
};

}

/**
 * キャッシュを持たない、シンプルなUniform
 */
class GLUniform : public internal::GLUniformBase {
public:
    /**
     * 44行列を転送する
     */
    bool upload(const mat4 &m, const GLboolean transpose = GL_FALSE);

};


}
}
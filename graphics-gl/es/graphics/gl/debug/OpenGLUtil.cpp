#include "es/OpenGL.hpp"
#include "es/internal/protoground-internal.hpp"

namespace es {
namespace gl {
namespace internal {


/**
 * 関連付けられたスレッドのglGetErrorを取り出す
 */
GLenum printError(const char *__file__, const int __line__) {
    const GLenum error = glGetError();
#define __prv__LOG_GL(err) case err: eslog_from(__file__, __line__, #err); break;
    switch (error) {
        __prv__LOG_GL(GL_INVALID_ENUM)
        __prv__LOG_GL(GL_INVALID_VALUE)
        __prv__LOG_GL(GL_INVALID_OPERATION)
        __prv__LOG_GL(GL_OUT_OF_MEMORY)
        __prv__LOG_GL(GL_INVALID_FRAMEBUFFER_OPERATION)
    }
#undef  __prv__LOG_GL
    return error;
}


}
}
}
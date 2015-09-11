#pragma once

#include "es/graphics/gl/shader/internal/GLUniformBase.h"

namespace es {
namespace gl {

class GLDevice;

class GLTexture;

class GLTextureUniform : public internal::GLUniformBase {
public:
public:
    GLTextureUniform() = default;

    bool upload(es::gl::GLDevice *device, es::gl::GLTexture *texture);

    ~GLTextureUniform() = default;

private:
    int bindUnit = -1;
};


}
}

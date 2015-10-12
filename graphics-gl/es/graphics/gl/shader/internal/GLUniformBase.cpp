#include "GLUniformBase.h"
#include "es/graphics/gl/resource/GLShaderProgram.h"

namespace es {
namespace gl {
namespace internal {

bool GLUniformBase::valid() const {
    return location >= 0;
}


void GLUniformBase::setLocation(const GLint location) {
    this->location = location;
}

GLint GLUniformBase::getLocation() const {
    return location;
}

bool GLUniformBase::setLocation(const std::shared_ptr<GLShaderProgram> program, const char *name) {
    location = program->getUniformLocation(name);
//        eslog("uniform location name(%s) -> %d", name, location);
    return valid();
}


}

/* es::gl::internal */

bool GLUniform::upload(const mat4 &m, const GLboolean transpose) {
    if (!valid()) {
        return false;
    }

    glUniformMatrix4fv(location, 1, transpose, (float *) &m);
    return true;
}

}
}
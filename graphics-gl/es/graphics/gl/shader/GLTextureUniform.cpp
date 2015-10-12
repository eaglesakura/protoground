#include "GLTextureUniform.h"
#include "es/graphics/gl/engine/GLDevice.h"
#include "es/graphics/gl/resource/GLTexture.h"

namespace es {
namespace gl {

bool GLTextureUniform::upload(es::gl::GLDevice *device, es::gl::GLTexture *texture) {
    assert(texture);
    assert(device);

    if (!valid()) {
        return false;
    }

	unsigned texUnitIndex = texture->bind(device->getTextureState());

    if (bindUnit != texUnitIndex) {
        glUniform1i(location, texUnitIndex);
        assert_gl();
        bindUnit = texUnitIndex;
        return true;
    }
    return false;
}

}
}
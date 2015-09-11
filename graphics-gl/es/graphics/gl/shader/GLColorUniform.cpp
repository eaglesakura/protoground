#include "GLColorUniform.h"

namespace es {
namespace gl {


GLColorUniform::GLColorUniform() {
    color.rgba = 0;
}

bool GLColorUniform::upload(const Color &c) {
    if (!valid()) {
        return false;
    }

    if (color != c) {
        glUniform4f(location, c.rf(), c.gf(), c.bf(), c.af());
        assert_gl();
        color = c;
        return true;
    }
    return false;
}

}
}
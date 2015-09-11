#pragma once

#include "es/graphics/gl/shader/internal/GLUniformBase.h"
#include "es/graphics/Color.hpp"

namespace es {
namespace gl {

class GLColorUniform : public internal::GLUniformBase {
public:
    GLColorUniform();

    /**
     * 色情報を転送する
     */
    bool upload(const Color &c);

    ~GLColorUniform() = default;
private:
    /**
     * 転送済みの色
     */
    Color color;
};


}
}
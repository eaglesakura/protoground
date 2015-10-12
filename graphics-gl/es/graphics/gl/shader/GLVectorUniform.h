#pragma once

#include "es/graphics/gl/shader/internal/GLUniformBase.h"
#include "es/math/protoground-glm.hpp"


namespace es {
namespace gl {

namespace internal {

template<typename vector_type, unsigned vector_length>
class GLVectorUniform : public GLUniformBase {
protected:
    GLVectorUniform();

    ~GLVectorUniform() = default;

    /**
     * 保持するベクトルユニット
     */
    vector_type vec[vector_length];
};

extern template
class GLVectorUniform<int32_t, 1>;

extern template
class GLVectorUniform<float, 1>;

extern template
class GLVectorUniform<float, 2>;

extern template
class GLVectorUniform<float, 3>;

extern template
class GLVectorUniform<float, 4>;

extern template
class GLVectorUniform<float, 3 * 3>;

extern template
class GLVectorUniform<float, 4 * 4>;

}

/**
 * uniform float
 * uniform float
 */
class GLIntUniform : public internal::GLVectorUniform<int32_t, 1> {
public:
    /**
     * float値を1つ転送する
     */
    bool upload(const int32_t x);
};

/**
 * uniform float
 * uniform float
 */
class GLFloatUniform : public internal::GLVectorUniform<float, 1> {
public:
    /**
     * float値を1つ転送する
     */
    bool upload(const float x);
};


/**
 * uniform float[2]
 * uniform vec2
 */
class GLVector2fUniform : public internal::GLVectorUniform<float, 2> {
public:
    /**
     * ベクトルを転送する
     */
    bool upload(const vec2 &v);

    /**
     * float値を2つ転送する
     */
    bool upload(const float x, const float y);
};

/**
 * uniform float[3]
 * uniform vec3
 */
class GLVector3fUniform : public internal::GLVectorUniform<float, 3> {
public:
    /**
     * ベクトルを転送する
     */
    bool upload(const vec3 &v);


    /**
     * float値を3つ転送する
     */
    bool upload(const float x, const float y, const float z);
};

/**
 * uniform float[4]
 * uniform vec4
 */
class GLVector4fUniofrm : public internal::GLVectorUniform<float, 4> {
public:
    /**
     * float値を4つ転送する
     */
    bool upload(const vec4 &v);

    /**
     * float値を4つ転送する
     */
    bool upload(const float x, const float y, const float z, const float w);
};

/**
 * uniform mat3
 */
class GLMatrix3Uniform : public internal::GLVectorUniform<float, 3 * 3> {
public:
    /**
     * 33行列を転送する
     */
    bool upload(const mat3 &m, const GLboolean transpose = GL_FALSE);
};

/**
 * uniform mat4
 */
class GLMatrix4Uniform : public internal::GLVectorUniform<float, 4 * 4> {
public:
    /**
     * 44行列を転送する
     */
    bool upload(const mat4 &m, const GLboolean transpose = GL_FALSE);
};

} /* es::gl */
} /* es */



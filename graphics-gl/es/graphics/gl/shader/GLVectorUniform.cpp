#include "GLVectorUniform.h"

namespace es {
namespace gl {
namespace internal {

template<typename vector_type, uint vector_length>
GLVectorUniform<vector_type, vector_length>::GLVectorUniform() {
    // 0で初期化する
    for (int i = 0; i < vector_length; ++i) {
        vec[i] = (vector_type) 0;
    }
}

template
class GLVectorUniform<int32_t, 1>;

template
class GLVectorUniform<float, 1>;

template
class GLVectorUniform<float, 2>;

template
class GLVectorUniform<float, 3>;

template
class GLVectorUniform<float, 4>;

template
class GLVectorUniform<float, 3 * 3>;

template
class GLVectorUniform<float, 4 * 4>;

bool uploadMatrixF(const GLint location, const float *vec, const float *pMatrix, const uint matrix_size, const uint matrix_num, const GLboolean transpose) {
    assert(pMatrix);

    const uint vector_length = (matrix_size * matrix_size); // 正方形マトリクス
    // 転送チェック
    if (memcmp(vec, pMatrix, sizeof(float) * vector_length) == 0) {
        // メモリ内容が一致したから何もしない
        return false;
    }

    // 行列の数だけ転送する
    switch (matrix_size) {
        case 4:
            glUniformMatrix4fv(location, vector_length / 16, transpose, pMatrix);
            break;
        case 3:
            glUniformMatrix3fv(location, vector_length / 9, transpose, pMatrix);
            break;
        case 2:
            glUniformMatrix2fv(location, vector_length / 4, transpose, pMatrix);
            break;
        default:
            // 非対応のサイズを転送しようとした
            assert(matrix_size != matrix_size);
            break;
    }
    assert_gl();
    return true;
}

}

bool GLIntUniform::upload(const int32_t x) {
    if (!valid()) {
        return false;
    }

    if (vec[0] != x) {
        glUniform1i(location, x);
        assert_gl();
        vec[0] = x;
        return true;
    }
    return false;
}

bool GLFloatUniform::upload(const float x) {
    if (!valid()) {
        return false;
    }

    if (vec[0] != x) {
        vec[0] = x;
        glUniform1fv(location, 1, vec);
        assert_gl();
        return true;
    }
    return false;
}

bool GLVector2fUniform::upload(const vec2 &v) {
    return upload(v.x, v.y);
}

bool GLVector2fUniform::upload(const float x, const float y) {
    if (!valid()) {
        return false;
    }

    if (vec[0] != x || vec[1] != y) {
        vec[0] = x;
        vec[1] = y;
        glUniform2fv(location, 1, vec);
        assert_gl();
        return true;
    }
    return false;
}

bool GLVector3fUniform::upload(const vec3 &v) {
    return upload(v.x, v.y, v.z);
}

bool GLVector3fUniform::upload(const float x, const float y, const float z) {
    if (!valid()) {
        return false;
    }

    if (vec[0] != x || vec[1] != y || vec[2] != z) {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
        glUniform3fv(location, 1, vec);
        assert_gl();
        return true;
    }
    return false;
}

bool GLVector4fUniofrm::upload(const vec4 &v) {
    return upload(v.x, v.y, v.z, v.w);
}

bool GLVector4fUniofrm::upload(const float x, const float y, const float z, const float w) {
    if (!valid()) {
        return false;
    }

    if (vec[0] != x || vec[1] != y || vec[2] != z || vec[3] != w) {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
        vec[3] = w;
        glUniform4fv(location, 1, vec);
        assert_gl();
        return true;
    }
    return false;
}

bool GLMatrix3Uniform::upload(const mat3 &m, const GLboolean transpose) {
    if (!valid()) {
        return false;
    }
    return internal::uploadMatrixF(location, vec, (const float *) (&m), 3, 1, transpose);
}

bool GLMatrix4Uniform::upload(const mat4 &m, const GLboolean transpose) {
    if (!valid()) {
        return false;
    }
    return internal::uploadMatrixF(location, vec, (const float *) (&m), 4, 1, transpose);
}

bool GLMatrix3Uniform::uploadNormal(const mat4 &m, const bool inverse, const bool transpose) {
    mat3 m33(m);
    if(inverse) {
        m33 = glm::inverse(m33);
    }
    return upload(m33, transpose);
}
}
}

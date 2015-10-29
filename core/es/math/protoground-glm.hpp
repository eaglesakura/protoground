#pragma once

#include "es/protoground.hpp"

#define GLM_FORCE_RADIANS

#if defined(BUILD_Windows_Cygwin)
#include <cmath>
#include <complex>
namespace std {
    template<typename T>
    inline T log2(T value) {
        return ::log2(value);
    }

    template<typename T>
    inline T trunc(T value) {
        return ::trunc(value);
    }

    template <typename T>
    inline T round(T value) {
        return ::round(value);
    }
}
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <cstring>

namespace es {

using ::glm::vec2;
using ::glm::vec3;
using ::glm::vec4;
using ::glm::mat3;
using ::glm::mat4;
using ::glm::quat;

using ::glm::i16vec2;
using ::glm::i16vec3;
using ::glm::i16vec4;
using ::glm::i32vec2;
using ::glm::i32vec3;
using ::glm::i32vec4;

/**
 * メモリサイズの検証
 */
static_assert(sizeof(float) == 4, "sizeof(float) != 4byte");
static_assert(sizeof(double) == 8, "sizeof(double) != 8byte");
static_assert(sizeof(mat4) == (sizeof(float) * 16), "sizeof(mat4) != float[4][4]");
static_assert(sizeof(vec2) == (sizeof(float) * 2), "sizeof(vec2) != float[2]");
static_assert(sizeof(vec3) == (sizeof(float) * 3), "sizeof(vec3) != float[3]");
static_assert(sizeof(vec4) == (sizeof(float) * 4), "sizeof(vec4) != float[4]");
static_assert(sizeof(quat) == (sizeof(float) * 4), "sizeof(vec4) != float[4]");
static_assert(sizeof(i16vec2) == (sizeof(int16_t) * 2), "sizeof(i16vec2) != int16_t[2]");
static_assert(sizeof(i16vec3) == (sizeof(int16_t) * 3), "sizeof(i16vec3) != int16_t[3]"); /* FIXME 暫定的にここでエラーにするようにしておく */
static_assert(sizeof(i16vec4) == (sizeof(int16_t) * 4), "sizeof(i16vec4) != int16_t[4]");

namespace util {

/**
 * vec4からvec3成分のみを取り出す
 */
inline void copyTo(vec3 *result, const vec4 &v) {
    memcpy(result, &v, sizeof(vec3));
}

template<typename T>
inline void setXYZ(T *result, const float value) {
    result->x = result->y = result->z = value;
}

template<typename T>
inline void setXYZ(T *result, const float x, const float y, const float z) {
    result->x = x;
    result->y = y;
    result->z = z;
}

template<typename T>
inline void setXYZW(T *result, const float x, const float y, const float z, const float w) {
    result->x = x;
    result->y = y;
    result->z = z;
    result->w = w;
}

inline void reset(vec3 *v) {
    memset(v, 0x00, sizeof(vec3));
}

inline void reset(quat *v) {
    memset(v, 0x00, sizeof(vec3));
    v->w = 1.0f;
}

/**
 * 行列の乗算をv3に対して行う
 */
inline void multiply(vec3 *result, const mat4 &m, const vec3 &v) {
    copyTo(result, m * vec4(v, 1));
}

/**
 * vec3メモリに強制変換する
 */
inline const vec3 &asVec3(const vec4 &v) {
    return *((vec3 *) (&v));
}

inline bool equals(const float a, const float b) {
    return std::abs(a - b) < 0.0000001f;
}

inline bool equals(const quat &a, const quat &b) {
    return equals(a.x, b.x) && equals(a.y, b.y) && equals(a.z, b.z) && equals(a.w, b.w);
}

inline bool equals(const vec3 &a, const vec3 &b) {
    return equals(a.x, b.x) && equals(a.y, b.y) && equals(a.z, b.z);
}

inline bool equals(const vec2 &a, const vec2 &b) {
    return equals(a.x, b.x) && equals(a.y, b.y);
}

}


namespace internal {
namespace debug {


/**
 * 左側4x3の部分しか利用していないことを検証する
 */
inline bool only3x4(const mat4 &m) {
    return ::es::util::equals(m[0][3], 0.0f) && ::es::util::equals(m[1][3], 0.0f) && ::es::util::equals(m[2][3], 0.0f) && ::es::util::equals(m[3][3], 1.0f);
}

inline bool valid(const float value) {
    if (value != value || value == INFINITY) {
        return false;
    } else {
        return true;
    }
}

inline bool valid(const vec4 &v) {
    return valid(v.x) && valid(v.y) && valid(v.z) && valid(v.w);
}

inline bool valid(const vec3 &v) {
    return valid(v.x) && valid(v.y) && valid(v.z);
}

inline bool valid(const quat &q) {
    if (!valid(q.x) || !valid(q.y) || !valid(q.z) || !valid(q.w)) {
        return false;
    }

    // 全てが0の場合は例外
    if (::es::util::equals(q.x, 0.0f) && ::es::util::equals(q.y, 0.0f) && ::es::util::equals(q.z, 0.0f) && ::es::util::equals(q.w, 0.0f)) {
        return false;
    }
    return true;
}

inline bool valid(const mat4 &m) {
    for (int i = 0; i < 4; ++i) {
        for (int k = 0; k < 4; ++k) {
            const float value = m[i][k];
            if (value != value // NaN
                || value == INFINITY) {
                return false;
            }
        }
    }
    return true;
}

}
}
}

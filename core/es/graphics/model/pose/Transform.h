#pragma once

#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"

namespace es {

struct Transform {
    float m[3][4];

    Transform();

    /**
     * 行列を変換する
     */
    Transform(const mat4 &org);

    /**
     * 行列を設定する
     */
    void set(const mat4 &org);

    /**
     * 回転部位を設定する
     */
    void set(const mat3 &org);

    /**
     * 逆行列を計算する
     * mat4x4を経由するので、動作は遅い。
     */
    void inverse(Transform *result) const;

    /**
     * mat4x4に変換する
     */
    void get4x4(float *result) const;

    /**
     * mat4に変換する
     */
    void get(mat4 *result) const;

    /**
     * スカラー積を計算する
     */
    inline void multiply(const float scalar) {
        for (int i = 0; i < 3; ++i) {
            for (int k = 0; k < 4; ++k) {
                m[i][k] *= scalar;
            }
        }
    }

    bool equals(const mat4 &m) const;

    inline void translate(const vec3 &pos) {
        m[0][3] = pos.x;
        m[1][3] = pos.y;
        m[2][3] = pos.z;
    }

    /**
     * 回転を行う
     */
    void rotate(const quat &q);
};

static_assert(sizeof(Transform) == sizeof(float) * 12, "sizeof(Transform) Error");

}



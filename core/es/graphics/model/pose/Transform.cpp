#include "Transform.h"
#include "es/internal/protoground-internal.hpp"

namespace es {


Transform::Transform() {
    for (int i = 0; i < 3; ++i) {
        for (int k = 0; k < 4; ++k) {
            m[i][k] = (i == k) ? 1 : 0;
        }
    }
}

Transform::Transform(const mat4 &org) {
    set(org);
}


void Transform::set(const mat4 &org) {
    assert(internal::debug::only3x4(org));

    for (int i = 0; i < 3; ++i) {
        for (int k = 0; k < 4; ++k) {
            m[i][k] = org[k][i];
        }
    }
}

void Transform::set(const mat3 &org) {
    for (int i = 0; i < 3; ++i) {
        for (int k = 0; k < 3; ++k) {
            m[i][k] = org[k][i];
        }
    }
}

void Transform::rotate(const quat &q) {
    set(glm::toMat3(q));
}

void Transform::inverse(Transform *result) const {
    result->set(glm::inverse(
            mat4(
                    m[0][0], m[1][0], m[2][0], 0,
                    m[0][1], m[1][1], m[2][1], 0,
                    m[0][2], m[1][2], m[2][2], 0,
                    m[0][3], m[1][3], m[2][3], 1
            )
    ));
}

void Transform::get4x4(float *result) const {
    for (int i = 0; i < 3; ++i) {
        for (int k = 0; k < 4; ++k) {
            result[k * 4 + i] = m[i][k];
        }
    }
}

void Transform::get(mat4 *result) const {
    get4x4((float *) result);
}

bool Transform::equals(const mat4 &org) const {
    // 再現不可能な行列の投入が行われたらロジックの問題である
    assert(internal::debug::only3x4(org));

    for (int i = 0; i < 3; ++i) {
        for (int k = 0; k < 4; ++k) {
            if (!util::equals(org[k][i], m[i][k])) {
                return false;
            }
        }
    }

    return true;
}


}
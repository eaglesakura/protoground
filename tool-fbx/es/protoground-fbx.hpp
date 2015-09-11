#pragma once

#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"
#include <fbxsdk.h>

namespace es {
namespace fbx {


/**
 * shared_ptrでラップする
 */
template<typename T>
std::shared_ptr<T> wrapFbxSharedPtr(T *ptr) {
    return std::shared_ptr<T>(ptr, [](T *ptr) {
        if (ptr) {
            ptr->Destroy();
        }
    });
}

}

namespace util {

inline vec3 to3dsAxis(const vec3 &v) {
    return vec3(v.x, v.z, -v.y);
}

inline void getTransform(vec3 *resultPosition, quat *resultQuat, vec3 *resultRotate, vec3 *resultScale, const FbxMatrix &m) {
    FbxVector4 translate;
    FbxQuaternion rotate;
    FbxVector4 sharing;
    FbxVector4 scale;
    double sign;

    m.GetElements(translate, rotate, sharing, scale, sign);
    if (resultPosition) {
        *resultPosition = vec3(translate[0], translate[1], translate[2]);
    }

    quat temp;
    temp.x = rotate[0];
    temp.y = rotate[1];
    temp.z = rotate[2];
    temp.w = rotate[3];
    if (resultQuat) {
        *resultQuat = temp;
    }
    if (resultRotate) {
        *resultRotate = glm::eulerAngles(temp);
    }
    if (resultScale) {
        *resultScale = vec3(scale[0], scale[1], scale[2]);
    }
}

inline void getTransform(vec3 *resultPosition, quat *resultQuat, vec3 *resultRotate, vec3 *resultScale, const FbxAMatrix &m) {
    getTransform(resultPosition, resultQuat, resultRotate, resultScale, FbxMatrix(m));
}

}

}
#include <es/math/Angle.hpp>
#include "SimpleSceneActor.h"

namespace es {

SimpleSceneActor::SimpleSceneActor() {
    rotate[0] = rotate[1] = rotate[2] = 0.0f;
    rotate[3] = 1.0f;
}

void SimpleSceneActor::calcWorld44(float *m44) const {
    (*((mat4 *) m44)) = glm::translate(pos) * getRotateMatrix() * glm::scale(vec3(scale, scale, scale));
}

float SimpleSceneActor::getScale() const {
    return scale;
}

void SimpleSceneActor::setScale(float scale) {
    SimpleSceneActor::scale = scale;
}

const vec3 &SimpleSceneActor::getPos() const {
    return pos;
}

void SimpleSceneActor::setPos(const vec3 &pos) {
    SimpleSceneActor::pos = pos;
}

void SimpleSceneActor::setRotate(const quat &rotate) {
    assert(rotateType == RotateType_Quat);
    memcpy(this->rotate, &rotate, sizeof(quat));
}

void SimpleSceneActor::setPos(const float x, const float y, const float z) {
    pos.x = x;
    pos.y = y;
    pos.z = z;
}

bool SimpleSceneActor::getCenter(float *v) const {
    memcpy(v, &pos, sizeof(vec3));
    return true;
}

void SimpleSceneActor::setRotateY(const float degree) {
    if (rotateType == RotateType_Quat) {
        setRotate(quat(vec3(0, glm::radians(degree), 0)));
    } else {
        rotate[1] = degree;
    }
}

void SimpleSceneActor::turnY(const float degree) {
    if (rotateType == RotateType_Quat) {
        setRotate(getRotateQuat() * quat(vec3(0, glm::radians(degree), 0)));
    } else {
        rotate[1] = util::normalizeDegree(rotate[1] + degree);
    }
}

void SimpleSceneActor::moveFront(const float length) {
    vec4 offset = getRotateMatrix() * vec4(0, 0, length, 1);
    pos.x += offset.x;
    pos.y += offset.y;
    pos.z += offset.z;
}

vec3 SimpleSceneActor::getRotateYawPitchRoll() const {
    return vec3(
            glm::degrees(glm::yaw(getRotateQuat())),
            glm::degrees(glm::pitch(getRotateQuat())),
            glm::degrees(glm::roll(getRotateQuat()))
    );
}

float SimpleSceneActor::getRotateYaw() const {
    return util::normalizeDegree(glm::degrees(glm::yaw(getRotateQuat())));
}

const quat &SimpleSceneActor::getRotateQuat() const {
    return *((quat *) rotate);
}

const vec3 &SimpleSceneActor::getRotateEuler() const {
    return *((vec3 *) rotate);
}

mat4 SimpleSceneActor::getRotateMatrix() const {
    switch (rotateType) {
        case RotateType_Y:
            return glm::rotate(glm::radians(rotate[1]), vec3(0, 1, 0));
        case RotateType_Quat:
            return glm::toMat4(getRotateQuat());
    }
    assert(false);
    return mat4();
}

SimpleSceneActor::RotateType_e SimpleSceneActor::getRotateType() const {
    return rotateType;
}

void SimpleSceneActor::setRotateType(const SimpleSceneActor::RotateType_e &rotateType) {
    SimpleSceneActor::rotateType = rotateType;
    rotate[0] = rotate[1] = rotate[2] = 0;
    rotate[3] = 1.0;
}

}
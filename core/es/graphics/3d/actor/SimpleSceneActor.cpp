#include <es/math/Angle.hpp>
#include "SimpleSceneActor.h"

namespace es {

SimpleSceneActor::SimpleSceneActor() {

}

void SimpleSceneActor::calcWorld44(float *m44) const {
    (*((mat4 *) m44)) = glm::translate(pos) * glm::toMat4(rotate) * glm::scale(vec3(scale, scale, scale));
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

const quat &SimpleSceneActor::getRotate() const {
    return rotate;
}

void SimpleSceneActor::setRotate(const quat &rotate) {
    SimpleSceneActor::rotate = rotate;
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
    rotate = quat(vec3(0, glm::radians(degree), 0));
}

void SimpleSceneActor::turnY(const float degree) {
    rotate *= quat(vec3(0, glm::radians(degree), 0));
}

void SimpleSceneActor::moveFront(const float length) {
    vec4 offset = glm::toMat4(rotate) * vec4(0, 0, length, 1);
    pos.x += offset.x;
    pos.y += offset.y;
    pos.z += offset.z;
}

vec3 SimpleSceneActor::getRotateYawPitchRoll() const {
    return vec3(
            glm::degrees(glm::yaw(rotate)),
            glm::degrees(glm::pitch(rotate)),
            glm::degrees(glm::roll(rotate))
    );
}

float SimpleSceneActor::getRotateYaw() const {
    return util::normalizeDegree(glm::degrees(glm::yaw(rotate)));
}
}
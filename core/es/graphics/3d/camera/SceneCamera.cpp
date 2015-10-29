#include "SceneCamera.h"

namespace es {

SceneCamera::SceneCamera() {
    setLens(sp<SceneLens>(new SceneLens()));
    pos = vec3(0, 10, 10);
    up = vec3(0, 1, 0);
}

void SceneCamera::setPos(const float x, const float y, const float z) {
    pos.x = x;
    pos.y = y;
    pos.z = z;
}

void SceneCamera::setLook(const float x, const float y, const float z) {
    look.x = x;
    look.y = y;
    look.z = z;
}

const vec3 &SceneCamera::getPos() const {
    return pos;
}

void SceneCamera::setPos(const vec3 &pos) {
    SceneCamera::pos = pos;
}

const vec3 &SceneCamera::getLook() const {
    return look;
}

void SceneCamera::setLook(const vec3 &look) {
    SceneCamera::look = look;
}

const vec3 &SceneCamera::getUp() const {
    return up;
}

void SceneCamera::setUp(const vec3 &up) {
    SceneCamera::up = up;
}

std::shared_ptr<SceneLens> SceneCamera::getLens() const {
    return lens;
}

void SceneCamera::setLens(const std::shared_ptr<SceneLens> &lens) {
    assert(lens);
    SceneCamera::lens = lens;
}

mat4 SceneCamera::calcLookMatrix() const {
    return glm::lookAtRH(pos, look, up);
}

mat4 SceneCamera::calcProjectionMatrix(const std::shared_ptr<IRenderingSurface> surface) const {
    return calcProjectionMatrix((float) surface->getWidth(), (float) surface->getHeight());
}

mat4 SceneCamera::calcProjectionMatrix(const float width, const float height) const {
    return glm::perspectiveFovRH(glm::radians(lens->getFovY()), width, height, near, far);
}

float SceneCamera::getNear() const {
    return near;
}

void SceneCamera::setNear(float near) {
    SceneCamera::near = near;
}

float SceneCamera::getFar() const {
    return far;
}

void SceneCamera::setFar(float far) {
    SceneCamera::far = far;
}

void SceneCamera::setRenderingRange(const float near, const float far) {
    this->near = near;
    this->far = far;
}

SceneCamera::ProjectionState SceneCamera::makeProjectionState() const {
    ProjectionState result;
    result.near = near;
    result.far = far;
    result.fovY = lens->getFovY();
    return result;
}

mat4 SceneCamera::ProjectionState::toMatrix(const float width, const float height) const {
    return glm::perspectiveFovRH(glm::radians(fovY), width, height, near, far);
}
}

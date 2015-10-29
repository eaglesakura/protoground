#pragma once

#include <es/graphics/engine/IRenderingSurface.hpp>
#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"
#include "SceneLens.h"

namespace es {

/**
 * レンダリング用カメラ
 *
 * レンズはデフォルトで35mm相当となる。
 */
class SceneCamera : public Object {
public:
    SceneCamera();

    struct ProjectionState {
        float near;
        float far;
        float fovY;

        mat4 toMatrix(const float width, const float height) const;
    };

    /**
     * Projection行列の情報をdumpする
     */
    ProjectionState makeProjectionState() const;

    mat4 calcLookMatrix() const;

    mat4 calcProjectionMatrix(const std::shared_ptr<IRenderingSurface> surface) const;

    mat4 calcProjectionMatrix(const float width, const float height) const;

    void setRenderingRange(const float near, const float far);

    float getNear() const;

    void setNear(float near);

    float getFar() const;

    void setFar(float far);

    void setPos(const float x, const float y, const float z);

    void setLook(const float x, const float y, const float z);

    const vec3 &getPos() const;

    void setPos(const vec3 &pos);

    const vec3 &getLook() const;

    void setLook(const vec3 &look);

    const vec3 &getUp() const;

    void setUp(const vec3 &up);

    std::shared_ptr<SceneLens> getLens() const;

    void setLens(const std::shared_ptr<SceneLens> &lens);

    virtual ~SceneCamera() = default;

private:
    /**
     * カメラ位置
     */
    vec3 pos;

    /**
     * カメラ注視点
     */
    vec3 look;

    /**
     * カメラの上向きベクトル
     */
    vec3 up;

    /**
     * レンズ設定
     */
    sp<SceneLens> lens;

    float near = 0.25f;

    float far = 1000.0f;
};

}



#pragma once

#include "es/protoground.hpp"
#include "SceneActor.h"

namespace es {

/**
 * 3D上の各種動作を実現するActor
 */
class SimpleSceneActor : public virtual SceneActor {
public:
    SimpleSceneActor();

    /**
     * 4x4行列でワールド行列を生成する
     */
    void calcWorld44(float *m44) const;

    float getScale() const;

    void setScale(float scale);

    const vec3 &getPos() const;

    void setPos(const float x, const float y, const float z);

    void setPos(const vec3 &pos);

    const quat &getRotate() const;

    /**
     * 向いている方向に対してlength分だけ移動させる
     */
    void moveFront(const float length);

    /**
     * ActorをY軸中心にオフセットさせる
     */
    void turnY(const float degree);

    /**
     * Y回転を指定する
     */
    void setRotateY(const float degree);

    void setRotate(const quat &rotate);

    /**
     * YPR角度を取得する。
     * この角度はdegreeとなる。
     */
    vec3 getRotateYawPitchRoll() const;

    float getRotateYaw() const;

    virtual bool getCenter(float *vec3) const;

    virtual ~SimpleSceneActor() = default;

protected:
    /**
     * スケーリング値
     */
    float scale = 1.0f;

    /**
     * 位置
     */
    vec3 pos;

    /**
     * 回転
     */
    quat rotate;
};


}

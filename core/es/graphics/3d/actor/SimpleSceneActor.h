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

    const quat &getRotateQuat() const;

    const vec3 &getRotateEuler() const;

    mat4 getRotateMatrix() const;

    /**
     * 向いている方向に対してlength分だけ移動させる
     */
    void moveFront(const float length);

    /**
     * ActorをY軸中心にオフセットさせる
     */
    void turnY(const float degree);

    /**
     * 指定回転角に向けて回転させる
     *
     * @return 回転を行ったらtrue、すでにtargetを向いていればfalseを返却する
     */
    bool turnToTargetY(const float degreeY, const float offset);

    /**
     * Y回転を指定する
     */
    void setRotateY(const float degree);

    float getRotateY() const;

    /**
     * 回転量を直接指定する
     */
    void setRotate(const quat &rotate);

    /**
     * YPR角度を取得する。
     * この角度はdegreeとなる。
     */
    vec3 getRotateYawPitchRoll() const;

    float getRotateYaw() const;

    virtual bool getCenter(float *vec3) const;

    virtual ~SimpleSceneActor() = default;

    enum RotateType_e {
        /**
         * Y軸のみで回転を行う
         */
                RotateType_Y,

        /**
         * クォータニオンで回転を行う
         */
                RotateType_Quat,
    };

    RotateType_e getRotateType() const;

    void setRotateType(const RotateType_e &rotateType);

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
    float rotate[4];

    /**
     * Yのみで回転
     */
    RotateType_e rotateType = RotateType_Y;
};


}

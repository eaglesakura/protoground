#pragma once

#include "es/protoground.hpp"
#include <btBulletDynamicsCommon.h>
#include <es/memory/SafeArray.hpp>
#include <es/graphics/model/file/ModelData.h>
#include <es/math/VectorMath.hpp>
#include "es/graphics/model/pose/BoneController.h"


namespace es {
namespace internal {

/**
 * Bullet形式の行列に変換する
 */
inline btTransform toBulletTransform(const mat4 &m) {
    btMatrix3x3 rotate(
            m[0][0], m[1][0], m[2][0],
            m[0][1], m[1][1], m[2][1],
            m[0][2], m[1][2], m[2][2]
    );
    return btTransform(rotate, btVector3(m[3][0], m[3][1], m[3][2]));
}

/**
 * モーション設定
 */
class MotionStateImpl : public btDefaultMotionState {
public:
    MotionStateImpl(const btTransform &startTrans, const btTransform &centerOfMassOffset)
            : btDefaultMotionState(startTrans, centerOfMassOffset) {
    }

    /**
     * Bulletに行列を設定する
     */
    virtual void getWorldTransform(btTransform &centerOfMassWorldTrans) const {
        centerOfMassWorldTrans = m_graphicsWorldTrans;
    }

    virtual ~MotionStateImpl() {
    }

    void setRigidDefaultWorldInv(const mat4 &rigidDefaultWorldInv) {
        MotionStateImpl::rigidDefaultWorldInv = rigidDefaultWorldInv;
    }


    void setRigidDefaultWorld(const mat4 &rigidDefaultWorld) {
        MotionStateImpl::rigidDefaultWorld = rigidDefaultWorld;
    }

    void setBoneTable(const sp<BoneTable> &boneTable, const int16_t boneIndex) {
        this->boneTable = boneTable;
        this->boneIndex = boneIndex;
    }

    /**
     * ボーン座標からBullet座標へ変換する
     */
    void linkFromBone() {
        m_graphicsWorldTrans = toBulletTransform(boneTable->palette[boneIndex] * rigidDefaultWorld);
    }

    /**
     * Bullet座標からボーンへ位置をフィードバックする
     */
    void feedbackToBone() const {
        m_graphicsWorldTrans.getOpenGLMatrix((btScalar *) (&boneTable->palette[boneIndex]));
        boneTable->palette[boneIndex] *= rigidDefaultWorldInv;
        cpu_math::mat44toTransform((float*)(boneTable->palette34.get(boneIndex)), (float*)(boneTable->palette.get(boneIndex)), 1);
    }

    bool isBoneLink() const {
        return boneLink;
    }

    void setBoneLink(bool boneLink) {
        MotionStateImpl::boneLink = boneLink;
    }

private:
    sp<BoneTable> boneTable;
    mat4 *globalMatrixRef = nullptr;
    mat4 rigidDefaultWorld;
    mat4 rigidDefaultWorldInv;
    int16_t boneIndex = -1;
    bool boneLink = false;
};

}
}
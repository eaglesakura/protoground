#pragma once

#include "es/protoground.hpp"
#include <btBulletDynamicsCommon.h>
#include <es/memory/SafeArray.hpp>
#include <es/graphics/model/file/ModelData.h>
#include <es/graphics/model/pose/PhysicsController.h>


namespace es {
namespace internal {


class ConstraintImpl : public btGeneric6DofSpringConstraint {
public:
    ConstraintImpl(btRigidBody &rbA, btRigidBody &rbB, const btTransform &frameInA, const btTransform &frameInB, bool useLinearReferenceFrameA)
            : btGeneric6DofSpringConstraint(rbA, rbB, frameInA, frameInB, useLinearReferenceFrameA) {
    }

    ConstraintImpl(btRigidBody &rbB, const btTransform &frameInB, bool useLinearReferenceFrameB)
            : btGeneric6DofSpringConstraint(rbB, frameInB, useLinearReferenceFrameB) {
    }

    virtual ~ConstraintImpl() = default;

    static sp <ConstraintImpl> newInstance(const file::PhysicsData::RigidJoint &joint, PhysicsController::Bind &main, PhysicsController::Bind *target) {
        ConstraintImpl *result = nullptr;

        StackBuffer<
                sizeof(mat4) * 5
        > buffer;

        mat4 &targetRigWorldInv = buffer.pop<mat4>();
        mat4 &mainRigWorldInv = buffer.pop<mat4>();
        mat4 &frameInMain = buffer.pop<mat4>();
        mat4 &jointWorld = buffer.pop<mat4>();

        // 初期座標を求める
        jointWorld = cpu_math::createMatrix(joint.position, joint.rotate);    // ジョイントの行列（モデルローカル座標系）

        main.body->getWorldMatrix(&mainRigWorldInv);
        mainRigWorldInv = glm::inverse(mainRigWorldInv);
        frameInMain = mainRigWorldInv * jointWorld;


        if (target) {
            mat4 &frameInTarget = buffer.pop<mat4>();

            // 拘束先がある
            target->body->getWorldMatrix(&targetRigWorldInv);
            targetRigWorldInv = glm::inverse(targetRigWorldInv);
            frameInTarget = targetRigWorldInv * jointWorld;

            // 拘束を生成する
            result = new ConstraintImpl(
                    *(main.body.get()), *(target->body.get()),
                    toBulletTransform(frameInMain), toBulletTransform(frameInTarget),
                    true
            );

        } else {
            // 単体拘束を生成する
            result = new ConstraintImpl(
                    *(main.body.get()), toBulletTransform(frameInMain), true
            );
        }

        for (int i = 0; i < 3; ++i) {
            if (joint.springTranslate[i] != 0) {
                result->enableSpring(i, true);
                result->setStiffness(i, joint.springTranslate[i]);
            }
            if (joint.springRotate[i] != 0) {
                result->enableSpring(i + 3, true);
                result->setStiffness(i + 3, joint.springRotate[i]);
            }
        }
        result->setLinearLowerLimit(btVector3(joint.translateMinLimit.x, joint.translateMinLimit.y, joint.translateMinLimit.z));
        result->setLinearUpperLimit(btVector3(joint.translateMaxLimit.x, joint.translateMaxLimit.y, joint.translateMaxLimit.z));
        result->setAngularLowerLimit(btVector3(joint.rotateMinLimit.x, joint.rotateMinLimit.y, joint.rotateMinLimit.z));
        result->setAngularUpperLimit(btVector3(joint.rotateMaxLimit.x, joint.rotateMaxLimit.y, joint.rotateMaxLimit.z));

        assert(result);
        return sp<ConstraintImpl>(result, deleter);
    }

private:
    static void deleter(btTypedConstraint *constraint) {
        if (!constraint) {
            return;
        }

        delete (constraint);
    }
};

}
}
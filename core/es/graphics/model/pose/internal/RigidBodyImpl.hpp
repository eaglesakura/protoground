#pragma once

#include "es/protoground.hpp"
#include <btBulletDynamicsCommon.h>
#include <es/memory/SafeArray.hpp>
#include <es/graphics/model/file/ModelData.h>
#include "MotionStateImpl.hpp"
#include "es/math/VectorMath.hpp"
#include "es/internal/protoground-internal.hpp"

namespace es {
namespace internal {

/**
 * RIG情報本体
 */
class RigidBodyImpl : public btRigidBody {
public:
    RigidBodyImpl(const btRigidBodyConstructionInfo &constructionInfo)
            : btRigidBody(constructionInfo) {
    }

    virtual ~RigidBodyImpl() {
    }

    /**
     * ワールド行列を取得する
     */
    void getWorldMatrix(mat4 *result) const {
        assert(result);
        StackBuffer<sizeof(btTransform)> buffer;
        btTransform &t = buffer.pop<btTransform>();

        getMotionState()->getWorldTransform(t);
        t.getOpenGLMatrix((btScalar *) result);
    }

    static sp<RigidBodyImpl> newInstance(const file::PhysicsData::RigidBody &body) {
        RigidBodyImpl *result = nullptr;


        btCollisionShape *shape = nullptr;
        switch (body.shapeType) {
            case file::PhysicsData::ShapeType_Box:
                shape = new btBoxShape(btVector3(body.size.x / 2, body.size.y / 2, body.size.z / 2));
                break;
            case file::PhysicsData::ShapeType_Sphere:
                shape = new btSphereShape(body.size.x);
                break;
            case file::PhysicsData::ShapeType_Capsule:
                shape = new btCapsuleShape(body.size.x, body.size.y);
                break;
            default:
                assert(false);
        }
        assert(shape);

        float mass;
        if (body.physicsFlags & file::PhysicsData::PhysicsFlag_BoneLink) {
            // ボーンリンクは摩擦無く動く
            mass = 0;
        } else {
            // 摩擦情報をコピーする
            mass = body.mass;
        }
        btVector3 localInertia(0, 0, 0);
        if (mass != 0) {
            shape->calculateLocalInertia(mass, localInertia);
        }

        // 位置を指定する
        MotionStateImpl *stateImpl = new MotionStateImpl(
                toBulletTransform(cpu_math::createMatrix(body.position, body.rotate)),
                btTransform::getIdentity()
        );

        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, stateImpl, shape, localInertia);
        result = new RigidBodyImpl(rbInfo);

        result->setRestitution(body.elasticity);
        result->setFriction(body.friction);
        result->setDamping(body.translateAttenuation, body.rotateAttenuation);

        if (body.physicsFlags & file::PhysicsData::PhysicsFlag_BoneLink) {
            // ボーン接続用のフラグを指定
            result->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT | result->getFlags());
        }
        result->setActivationState(DISABLE_DEACTIVATION);


        // 計算用の初期座標行列をコピーする
        {
            StackBuffer<
                    sizeof(mat4) * 1
            > buffer;
            mat4 &tempWorld = buffer.pop<mat4>();

            result->getWorldMatrix(&tempWorld);
            stateImpl->setRigidDefaultWorld(tempWorld);
            stateImpl->setRigidDefaultWorldInv(glm::inverse(tempWorld));
        }

        assert(result);
        return sp<RigidBodyImpl>(result, RigidBodyImpl::deleter);
    }

private:
    /**
     * 軽量なdeleter
     * selfが必要ない場合はこちらのほうが軽量
     */
    static void deleter(btRigidBody *body) {
        if (!body) {
            return;
        }

        btMotionState *state = body->getMotionState();
        btCollisionShape *shape = body->getCollisionShape();

        util::safe_delete(&shape);
        util::safe_delete(&state);
        util::safe_delete(&body);
    }
};


}
}
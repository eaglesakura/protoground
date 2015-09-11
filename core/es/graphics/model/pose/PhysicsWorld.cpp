#include "PhysicsWorld.h"
#include <btBulletDynamicsCommon.h>

namespace es {

void PhysicsWorld::setGravity(const float basicCharactorHeightMeter, const float basicCharactorVertexHeight) {
    assert(dynamicsWorld);
    dynamicsWorld->setGravity(btVector3(
            0,
            -(9.80665 * basicCharactorVertexHeight / basicCharactorHeightMeter),
            0
    ));
}

/**
 * デフォルトのワールドを生成する
 */
void PhysicsWorld::alloc() {
    configuration.reset(new btDefaultCollisionConfiguration());
    dispatcher.reset(new btCollisionDispatcher(configuration.get()));
    broadphaseInterface.reset(new btDbvtBroadphase());
    constraintSolver.reset(new btSequentialImpulseConstraintSolver());
    dynamicsWorld.reset(new btDiscreteDynamicsWorld(dispatcher.get(), broadphaseInterface.get(), constraintSolver.get(), configuration.get()));

    setGravity();

    // 正常にallocが終わっていることを保証する
    allocated = true;
}

void PhysicsWorld::attach(btRigidBody *object, uint16_t group, uint16_t groupMask) {
    assert(allocated);
    if (!object) {
        return;
    }
    dynamicsWorld->addRigidBody(object, group, groupMask);
}

void PhysicsWorld::detach(btRigidBody *object) {
    assert(allocated);
    if (!object) {
        return;
    }
    dynamicsWorld->removeRigidBody(object);
}

void PhysicsWorld::attach(btTypedConstraint *constraint) {
    assert(allocated);
    if (!constraint) {
        return;
    }
    dynamicsWorld->addConstraint(constraint);
}

void PhysicsWorld::detach(btTypedConstraint *constraint) {
    assert(allocated);
    if (!constraint) {
        return;
    }
    dynamicsWorld->removeConstraint(constraint);
}

void PhysicsWorld::update(const float deltaTimeSec) {
    dynamicsWorld->stepSimulation(deltaTimeSec, maxSubStep, (btScalar) fixedTimeStep);
}

void PhysicsWorld::debugDrawWorld(btIDebugDraw *debugDrawer) {
    assert(debugDrawer);

    dynamicsWorld->setDebugDrawer(debugDrawer);
    dynamicsWorld->debugDrawWorld();
    dynamicsWorld->setDebugDrawer(nullptr);
}

}
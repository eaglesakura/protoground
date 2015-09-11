#include "PhysicsController.h"
#include "es/graphics/model/pose/PhysicsWorld.h"
#include "internal/MotionStateImpl.hpp"
#include "internal/RigidBodyImpl.hpp"
#include "internal/ConstraintImpl.hpp"

namespace es {


PhysicsController::PhysicsController() {
}

PhysicsController::~PhysicsController() {
    this->detachWorld();
}


void PhysicsController::initialize(const std::shared_ptr<file::SkinMeshModelData> model, const std::shared_ptr<BoneTable> table) {
    assert(model);
    assert(!this->model);
    this->model = model;
    this->boneTable = table;

    util::valloc(&binds, model->physics.rigidBodies.size(), false);
    // 剛体を生成する
    {
        int index = 0;
        for (const auto &src : model->physics.rigidBodies) {
            auto &dst = binds[index++];
            assert(src.linkBoneIndex >= 0);
            assert(src.linkBoneIndex < model->bone.bones.size());

            dst.linkedBoneIndex = src.linkBoneIndex;
            dst.body = internal::RigidBodyImpl::newInstance(src);
            ((internal::MotionStateImpl *) dst.body->getMotionState())->setBoneTable(table, src.linkBoneIndex);
        }
    }

    // ジョイントを生成する
    {
        for (const auto &src : model->physics.rigidJoints) {
            assert(src.rigidBodyIndex >= 0);
            assert(src.rigidBodyIndex < binds.size());
            assert(src.connectRigidBodyIndex < 0 ? true : src.rigidBodyIndex < binds.size());

            PhysicsController::Bind &main = binds[src.rigidBodyIndex];
            PhysicsController::Bind *target = src.connectRigidBodyIndex < 0 ? nullptr : &binds[src.connectRigidBodyIndex];
            auto constraint = internal::ConstraintImpl::newInstance(src, main, target);
            assert(constraint);
            main.constraints.push_back(constraint);
        }
    }

    // バインドのメモリを整理する
    for (auto &bind : binds) {
        bind.constraints.shrink_to_fit();
    }
}

void PhysicsController::attachWorld(const std::shared_ptr<PhysicsWorld> &world) {
    assert(model);
    // 古いワールドを切り離す
    if (!this->world) {
        detachWorld();
    }

    this->world = world;
    if (!world) {
        return;
    }

    int index = 0;
    for (auto &bind : binds) {
        const auto &rig = model->physics.rigidBodies[index++];
        world->attach(bind.body.get(), rig.groupFlags, rig.noCollisionGroups);
    }

    for (auto &bind : binds) {
        for (auto &cts : bind.constraints) {
            world->attach(cts.get());
        }
    }
}

void PhysicsController::detachWorld() {
    if (!world) {
        return;
    }

    for (auto &bind : binds) {
        for (auto &cts : bind.constraints) {
            world->detach(cts.get());
        }
    }

    for (auto &bind : binds) {
        world->detach(bind.body.get());
    }

    world.reset();
}

void PhysicsController::linkFromBone(bool forceUpdate) {
    int index = 0;
    for (auto &bind : binds) {
        const auto &physics = model->physics.rigidBodies[index++];

        auto state = (internal::MotionStateImpl *) bind.body->getMotionState();
        if (forceUpdate) {
            // 強制的に更新
            state->linkFromBone();
        } else {
            if (physics.physicsFlags & file::PhysicsData::PhysicsFlag_BoneLink) {
                // ボーンと紐付いたものだけを更新
                state->linkFromBone();
            }
        }
    }

}

void PhysicsController::feedbackToBone() {
    int index = 0;
    for (auto &bind : binds) {
        const auto &physics = model->physics.rigidBodies[index++];
        auto state = (internal::MotionStateImpl *) bind.body->getMotionState();
        if (!(physics.physicsFlags & file::PhysicsData::PhysicsFlag_BoneLink)) {
            // ボーンと紐付いたものだけを更新
            state->feedbackToBone();
        }
    }
}

const std::shared_ptr<PhysicsWorld> &PhysicsController::getWorld() const {
    return world;
}

}
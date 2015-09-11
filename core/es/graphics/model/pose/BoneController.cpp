#include "BoneController.h"
#include "es/graphics/model/pose/AnimationPose.h"
#include "btBulletCollisionCommon.h"
#include "es/internal/protoground-internal.hpp"
#include "es/math/VectorMath.hpp"
#include "PhysicsWorld.h"
#include "PhysicsController.h"

namespace es {

BoneController::BoneController() {

}

void BoneController::initialize(const std::shared_ptr<file::SkinMeshModelData> &model) {
    assert(model);
    if (model->bone.bones.empty()) {
        return;
    }
    this->model = model;

    util::valloc(&boneCaches, model->bone.bones.size(), false);
    util::valloc(&globalTable, model->bone.bones.size(), false);
    boneTable.reset(new BoneTable());
    boneTable->palette.setBuffer(Buffer::alignNew(sizeof(mat4) * model->bone.bones.size()));
    boneTable->palette34.setBuffer(Buffer::alignNew(sizeof(Transform) * model->bone.bones.size()));
    int index = 0;
    for (const auto &src : model->bone.bones) {
        auto &dst = boneCaches[index];

        if (src.parentBoneIndex >= 0) {
            // 親に自分の存在を知らせる
            const auto &srcParent = model->bone.bones[src.parentBoneIndex];
            auto &dstParent = boneCaches[src.parentBoneIndex];
            dstParent.childs.push_back(index);

            dst.offsetParent = glm::translate(src.pos - srcParent.pos);
        } else {
            dst.offsetParent = glm::translate(src.pos);
        }
        dst.invert = glm::inverse(glm::translate(src.pos) * glm::toMat4(src.rotate));
        ++index;
    }

    assert(!boneCaches.empty());
    assert(globalTable.size() == boneCaches.size());

    resetPose();
}

void BoneController::bind(std::shared_ptr<PhysicsWorld> &world, std::shared_ptr<PhysicsController> &physics) {
    assert(world);
    assert(physics);

    this->physics = physics;
    physics->initialize(model, boneTable);
    physics->attachWorld(world);
}

void BoneController::update(const AnimationPose *pose) {
    if (!pose) {
        // ポーズが無効な場合は全てリセットする
        resetPose();
        return;
    }
    updateWorldMatrixFKLocal(pose);
    // ボーンがIK情報を持つなら、IK計算を行う
    if (!model->bone.ikLinks.empty()) {
        updateWorldMatrixIKLocal();
    }
    updateBoneMatrixTable();
}

void BoneController::updateWithPhysics(const AnimationPose *pose, const float deltaTimeSec) {
    update(pose);

    assert(physics);
    if (deltaTimeSec > 0) {
        // 物理更新
        physics->linkFromBone(false);
        physics->getWorld()->update(deltaTimeSec);
        physics->feedbackToBone();
    }
}

void BoneController::resetPose() {
    const mat4 identity[BoneMatrix::CalcPass_World];
    for (auto &world : globalTable) {
        world.pass = BoneMatrix::CalcPass_Local;
        memcpy(world.cache, identity, sizeof(identity));
    }

    updateBoneMatrixTable();
}


void BoneController::updateWorldMatrixFKLocal(const AnimationPose *pose) {
    for (auto &world : globalTable) {
        // 未計算に戻す
        world.pass = BoneMatrix::CalcPass_None;
    }

    StackBuffer<sizeof(mat4)> buffer;
    mat4 &translate = buffer.pop<mat4>();
    translate = mat4();

    // 全てのローカル行列を更新する
    {
        int index = 0;
        for (auto &world : globalTable) {
            const auto &cache = boneCaches[index];
            const auto &bind = pose->binds[index];

//            eslog("Bind[%03d] quat(%.3f, %.3f, %.3f, %.3f)", index,
//                  pose->binds[index].rotate.x, pose->binds[index].rotate.y, pose->binds[index].rotate.z, pose->binds[index].rotate.w
//            );

            if (!(bind.flags & AnimationPose::Bind::Flag_Unuse)) {
                // 計算対象ならアニメーション由来で計算する
                cpu_math::setMat44Translate((float *) &translate, (float *) &bind.translate);
                if (pose->anim->animation.meta.flags & file::AnimationData::Meta::Flag_BoneHasParentOffset) {
                    world.cache[BoneMatrix::CalcPass_Local] = translate;
                } else {
                    memcpy(&world.cache[BoneMatrix::CalcPass_Local], &cache.offsetParent, sizeof(mat4));
                    world.cache[BoneMatrix::CalcPass_Local] *= translate;
                }
                world.cache[BoneMatrix::CalcPass_Local] *= glm::toMat4(bind.rotate);
            } else {
                // 計算対象でないのでデフォルトの行列を適用する
                world.cache[BoneMatrix::CalcPass_Local] = cache.offsetParent;
            }

            world.pass = BoneMatrix::CalcPass_Local;
            ++index;
        }
    }
}


void BoneController::updateWorldMatrixIKLocal() {

    StackBuffer<
            sizeof(mat4) * 2 +
            sizeof(vec4) * (4 + 3)
    > buffer;

    mat4 &tempLinkBoneInv = buffer.pop<mat4>();
    mat4 &rotation = buffer.pop<mat4>();

    vec4 &currentPos = buffer.pop<vec4>();
    vec4 &targetPos = buffer.pop<vec4>();
    vec4 &localCurrentPos = buffer.pop<vec4>();
    vec4 &localTargetPos = buffer.pop<vec4>();

    vec3 &localDir = buffer.pop<vec3>(sizeof(vec4));
    vec3 &localTargetDir = buffer.pop<vec3>(sizeof(vec4));
    vec3 &axis = buffer.pop<vec3>(sizeof(vec4));

    currentPos.w = targetPos.w = 1.0f;

    uint index = 0;
    const uint numBones = model->bone.bones.size();
    for (int i = 0; i < numBones; ++i) {
        const auto &selfBone = model->bone.bones[i];
        const auto &selfBoneCache = boneCaches[i];
        const auto &selfWorld = globalTable[i];
        if (!selfBone.ikLinkNum) {
            // IKリンクが存在しない
            continue;
        }

        auto ikLinks = util::unsafe(model->bone.ikLinks, selfBone.ikLinkOffset, selfBone.ikLinkNum);
        assert(ikLinks.length);

        assert(selfBone.ikTargetBoneIndex >= 0);
        assert(selfBone.ikTargetBoneIndex < numBones);

        const mat4 &currentBoneGlobal = calcWorldMatrix(i);

        for (int loop = (selfBone.ikLoopCount - 1); loop >= 0; --loop) {
            auto ikLinkCollection = util::unsafe(model->bone.ikLinks, selfBone.ikLinkOffset, selfBone.ikLinkNum);

            while (ikLinkCollection) {
                auto &link = *ikLinkCollection;
                ++ikLinkCollection;

                assert(link.linkBoneIndex >= 0);

                auto &linkBone = model->bone.bones[link.linkBoneIndex];
                const mat4 &targetBoneGlobal = calcWorldMatrix(selfBone.ikTargetBoneIndex);

                heavy_assert(internal::debug::valid(targetBoneGlobal));

                currentPos.x = targetBoneGlobal[3][0];
                currentPos.y = targetBoneGlobal[3][1];
                currentPos.z = targetBoneGlobal[3][2];

                targetPos.x = currentBoneGlobal[3][0];
                targetPos.y = currentBoneGlobal[3][1];
                targetPos.z = currentBoneGlobal[3][2];

                tempLinkBoneInv = glm::inverse(calcWorldMatrix(link.linkBoneIndex));
                heavy_assert(debug::only3x4(tempLinkBoneInv));

                cpu_math::multMat44Vec4((float *) &localCurrentPos, (float *) &currentPos, (float *) &tempLinkBoneInv);
                cpu_math::multMat44Vec4((float *) &localTargetPos, (float *) &targetPos, (float *) &tempLinkBoneInv);

                localDir = glm::normalize(util::asVec3(localCurrentPos));
                localTargetDir = glm::normalize(util::asVec3(localTargetPos));
                assert(internal::debug::valid(localDir));
                assert(internal::debug::valid(localTargetDir));

                if (glm::length(localTargetDir - localDir) < 0.001f) {
                    // ほぼ同一座標にいるので、計算対象外
                    break;
                }

                float p = glm::dot(localDir, localTargetDir);
                // 値は有効であるはず
                assert(internal::debug::valid(p));
                if (std::abs(p) >= 1) {
                    continue;
                }

                // 角度チェックと強制
                float angle = acos(p);
                assert(internal::debug::valid(angle));

                angle = std::min(angle, selfBone.ikLimitedRadian);
                axis = -glm::cross(localTargetDir, localDir);
                axis = glm::normalize(axis);
                rotation = glm::rotate(angle, axis);

                mat4 &linkBoneLocalRef = globalTable[link.linkBoneIndex].cache[BoneMatrix::CalcPass_Local];
                if (link.flags & file::BoneData::IkLink::Flag_RotateLimited) {
                    const quat desired_rotation(linkBoneLocalRef * rotation);
                    const vec3 desired_euler = glm::eulerAngles(desired_rotation);
                    const vec3 clamped_euler = glm::clamp(desired_euler, link.minRadian, link.maxRadian);
                    const quat clamped_rotation(clamped_euler);
                    cpu_math::setMat44Rotate33(&linkBoneLocalRef, glm::toMat3(clamped_rotation));
                    heavy_assert(debug::valid(clamped_rotation));
                } else {
                    linkBoneLocalRef *= rotation;
                }
            }
        }
    }
}

void BoneController::updateBoneMatrixTable() {

    {
        // 全てのグローバル行列を更新する
        // 計算回数を抑えるため、複数実行
        bool requestNextPass;
        int passCount = 0;
        do {
            requestNextPass = false;

            int _index = 0;
            for (auto &world : globalTable) {
                const int index = _index++;
                // ワールド行列計算済みなら何もしない
                if (world.pass == BoneMatrix::CalcPass_World) {
                    continue;
                }
                const auto &bone = model->bone.bones[index];
                if (bone.parentBoneIndex >= 0) {
                    // 親を持つなら親をチェックする
                    const auto parentWorld = globalTable[bone.parentBoneIndex];
                    if (parentWorld.pass != BoneMatrix::CalcPass_World) {
                        // ワールド計算が完了するまで待つ
                        requestNextPass = true;
                        continue;
                    }

                    world.cache[BoneMatrix::CalcPass_World] = parentWorld.cache[BoneMatrix::CalcPass_World] * world.cache[BoneMatrix::CalcPass_Local];
                } else {
                    // ボーン全体のワールドと掛け合わせる
                    world.cache[BoneMatrix::CalcPass_World] = this->world * world.cache[BoneMatrix::CalcPass_Local];
                }
                world.pass = BoneMatrix::CalcPass_World;
            }
            ++passCount;
        } while (requestNextPass);
    }

    // 実際に適用すべき行列を生成する
    {
        const int numTable = globalTable.size();
        for (int i = 0; i < numTable; ++i) {
            boneTable->palette[i] = globalTable[i].cache[BoneMatrix::CalcPass_World] * boneCaches[i].invert;
        }
        cpu_math::mat44toTransform((float *) boneTable->palette34.get(), (float *) boneTable->palette.get(), numTable);
    }
}

/**
 * ワールド行列を再計算する
 */
mat4 BoneController::calcWorldMatrix(const uint boneIndex) const {
    const auto &bone = model->bone.bones[boneIndex];
    if (bone.parentBoneIndex < 0) {
        return world * globalTable[boneIndex].cache[BoneMatrix::CalcPass_Local];
    } else {
        return calcWorldMatrix(bone.parentBoneIndex) * globalTable[boneIndex].cache[BoneMatrix::CalcPass_Local];
    }
}

void BoneController::debugDraw(btIDebugDraw *drawer) const {
    assert(drawer);

    for (int i = 0; i < boneCaches.size(); ++i) {
        const auto &bone = model->bone.bones[i];
        const auto &cache = boneCaches[i];

        btVector3 color(1, 0, 0);
        if (bone.ikLinkNum) {
            color = btVector3(1, 1, 0);
        }

        vec3 to = util::asVec3(boneTable->palette[i] * vec4(bone.pos, 1.0f));
        vec3 from;
        if (bone.parentBoneIndex >= 0) {
            // 親座標をコピーする
            from = util::asVec3(
                    boneTable->palette[bone.parentBoneIndex] *
                    vec4(model->bone.bones[bone.parentBoneIndex].pos, 1.0f)
            );
        } else {
            from = util::asVec3(
                    world *
                    vec4(0.0f, 0.0f, 0.0f, 1.0f)
            );
        }
//        eslog("Bone pos(%.3f, %.3f, %.3f)", from.x, from.y, from.z);
        vec3 center = (to + from) / 2.0f;
        drawer->drawLine(btVector3(from.x, from.y, from.z), btVector3(center.x, center.y, center.z), color);
        drawer->drawLine(btVector3(center.x, center.y, center.z), btVector3(to.x, to.y, to.z), btVector3(0.25f, 0.25f, 0.0f));
    }
}

void BoneController::setWorld(const mat4 &world) {
    BoneController::world = world;
}

const mat4 &BoneController::getWorld() const {
    return world;
}

unsafe_array<Transform> BoneController::getBoneTable34() const {
    return boneTable->palette34.unsafe(boneCaches.size());
}

}
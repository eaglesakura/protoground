#include "AnimationPose.h"
#include "es/internal/protoground-internal.hpp"

namespace es {

AnimationPose::AnimationPose(sp<file::SkinMeshModelData> model) : model(model) {
    assert(model);

    // リンクデータを構築する
    util::valloc(&binds, model->bone.bones.size(), true);
}

void AnimationPose::bind(const std::shared_ptr<file::AnimationData> &anim) {
    assert(anim);
    if (this->anim == anim) {
        // 同じアニメーションならば、フレームをリセットするだけで問題ない
        this->frame = 0;
        return;
    }

    this->anim = anim;
    this->frame = 0;

    // 全てのアニメーションのインデックスを探す
    int boneIndex = 0;
    for (const auto &src : model->bone.bones) {
        auto &dst = binds[boneIndex++];

        dst.linkBoneTimelineIndex = findBoneTimelineIndex(src.symbol);
        if (dst.linkBoneTimelineIndex < 0) {
            // リンクが成立しなかった
//            eslog("Skip Bone[%03d]", boneIndex - 1);
            dst.flags = Bind::Flag_Unuse;
        } else {
//            eslog("Link Bone[%03d] -> Anim[%03d]", boneIndex - 1, dst.linkBoneTimelineIndex);
            dst.flags = 0;
        }
    }
}

/**
 * データ更新する
 */
void AnimationPose::update(const float frame) {
    assert(anim);
    assert(!binds.empty());
    this->frame = normalizeFrame(frame);

    // 全てのボーンを更新する
    {
        int index = 0;
        for (auto &bind: binds) {
            if (bind.linkBoneTimelineIndex >= 0) {
                const auto &timeline = anim->animation.linkBones[bind.linkBoneTimelineIndex];
                update(&bind, model->bone.bones[index], timeline);
            }
            ++index;
        }
    }
}

/**
 * 1つのボーンタイムラインを更新する
 */
void AnimationPose::update(AnimationPose::Bind *result, const file::BoneData::Bone &bone, const file::AnimationData::SymbolTimeline &timeline) {
    assert(result);

    auto keys = util::unsafe(anim->animation.boneKeys, timeline.offset, timeline.num);
    file::AnimationData::BoneKeyFrame *start = nullptr;
    file::AnimationData::BoneKeyFrame *end = nullptr;
    float weight = 0;


    findKeyFrame<file::AnimationData::BoneKeyFrame>(keys, &start, &end, &weight);
    if (start && end) {
        if (weight > 0 && weight < 1) {

            result->translate.x = (end->pos.x * weight) + (start->pos.x * (1.0f - weight));
            result->translate.y = (end->pos.y * weight) + (start->pos.y * (1.0f - weight));
            result->translate.z = (end->pos.z * weight) + (start->pos.z * (1.0f - weight));

            result->rotate = glm::slerp(start->rotate, end->rotate, weight);
        } else if (weight == 0) {
            result->translate = start->pos;
            result->rotate = start->rotate;
        } else {
            result->translate = end->pos;
            result->rotate = end->rotate;
        }
    }
}


int16_t AnimationPose::findBoneTimelineIndex(const uint64_t symbol) const {
    assert(anim);
    int16_t index = 0;
    for (const auto &src : anim->animation.linkBones) {
        if (src.symbol == symbol) {
            return index;
        }
        ++index;
    }

    return -1;
}

float AnimationPose::normalizeFrame(const float newFrame) const {
    float result = newFrame;
    if (result < 0) {
        result += (float) anim->animation.meta.endFrame;
    }
    if (result > anim->animation.meta.endFrame) {
        result -= anim->animation.meta.endFrame;
    }
    return result;
}

float AnimationPose::getFrame() const {
    return frame;
}

const std::shared_ptr<file::AnimationData> &AnimationPose::getAnimationData() const {
    return anim;
}
}
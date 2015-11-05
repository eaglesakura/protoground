#pragma once

#include <vector>
#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"
#include "es/graphics/model/file/ModelData.h"
#include "es/graphics/model/file/AnimationData.h"
#include "BoneController.h"

namespace es {

class SkinningModel;

class BoneController;

/**
 * アニメーション中の1ポーズを構築する。
 * ポーズ用のメモリはモデルごとに固有のサイズとなる（ボーン数に依存する）
 *
 * ボーン数とアニメーション内のタイムライン数は通常一致しない（存在しないボーンに対するアニメーション、等）。
 * そのため、アニメーション処理前にSymbolを関連付ける必要がある。
 *
 * TODO: 複数アニメーションのブレンドに対応する
 */
class AnimationPose : public Object {
public:
    AnimationPose(sp<file::SkinMeshModelData> model);

    /**
     * アニメーションと関連付ける
     *
     * アニメーションを切り替える度に呼び出す必要がある。
     */
    virtual void bind(const std::shared_ptr<file::AnimationData> &anim);

    /**
     * フレームを指定してポーズを更新する
     *
     * @param frame アニメーションのフレーム番号
     */
    virtual void update(const float frame);

    /**
     * 現在のアニメーションフレーム番号を取得する
     */
    float getFrame() const;


    /**
     * 元のアニメーション情報を取得する
     */
    const std::shared_ptr<file::AnimationData> &getAnimationData() const;

    virtual ~AnimationPose() = default;

protected:
    struct Bind;

    /**
     * ボーンを更新する
     */
    void update(Bind *result, const file::BoneData::Bone &bone, const file::AnimationData::SymbolTimeline &timeline);

    /**
     * ポーズのボーン事のバインドを構築する。
     * モデルに内包される1ボーンごとに1つ生成される。、
     */
    struct Bind {
        /**
         * 移動量
         */
        vec3 translate;

        /**
         * 回転量
         */
        quat rotate;

        enum {
            /**
             * このバインドが利用されず、計算の必要がないことを示す
             */
                    Flag_Unuse = 0x1 << 0,
        };

        /**
         * 計算フラグ
         */
        uint16_t flags;

        /**
         * リンクされたアニメーションデータのインデックス
         * リンクされない場合は負の値が指定される。
         *
         * AnimationData::Serialize::linkBones[linkBonesIndex]
         */
        int16_t linkBoneTimelineIndex;
    };

    /**
     * バインド情報
     */
    std::vector<Bind> binds;

    /**
     * モデル情報
     */
    sp<file::SkinMeshModelData> model;

    /**
     * 現在のアニメーションフレーム
     */
    float frame = 0;

    /**
     * 元データ
     */
    sp<file::AnimationData> anim;

    /**
     * シンボル名からアニメーションを探す
     *
     * 見つからない場合は負の値を返却する。
     */
    int16_t findBoneTimelineIndex(const uint64_t symbol) const;

    /**
     * 
     */
    template<typename KeyFrameValue>
    void findKeyFrame(unsafe_array<KeyFrameValue> keys, KeyFrameValue **start, KeyFrameValue **end) const {
        assert(start);
        assert(end);

        if (!keys) {
            *start = *end = nullptr;
        } else if (keys.length == 1) {
            *start = *end = (KeyFrameValue *) &keys[0];
            return;
        }

        // check frame
        const int16_t checkFrame = (int16_t) ::ceil(frame);

        KeyFrameValue *before = nullptr;
        KeyFrameValue *beforeBefore = nullptr;
        while (keys) {
            const KeyFrameValue &key = *keys;

            if (key.frame >= checkFrame) {
                *end = (KeyFrameValue *) &key;
                if (before) {
                    *start = before;
                } else {
                    *start = *end;
                }
                return;
            } else {
                beforeBefore = before;
                before = (KeyFrameValue *) &key;
            }

            ++keys;
        }

        if (!beforeBefore) {
            *start = *end = before;
        } else {
            *start = beforeBefore;
            *end = before;
        }
    }

    template<typename KeyFrameValue>
    void findKeyFrame(unsafe_array<KeyFrameValue> keys, KeyFrameValue **start, KeyFrameValue **end, float *weight) const {
        assert(weight);

        findKeyFrame<KeyFrameValue>(keys, start, end);

        assert(*start);
        assert(*end);
        assert((*start)->frame <= (*end)->frame);

        if (frame <= (float) (*start)->frame) {
            *weight = 0;
        } else if (frame >= (float) (*end)->frame) {
            *weight = 1;
        } else {
            *weight = (frame - (float) (*start)->frame) / (float) ((*end)->frame - (*start)->frame);
        }
        assert((*weight) >= 0 && (*weight) <= 1);
    }


    /**
     * フレーム数を正規化する
     */
    float normalizeFrame(const float newFrame) const;

    friend class SkinningModel;

    friend class BoneController;
};

}


#pragma once

#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"
#include <vector>
#include <es/memory/Buffer.hpp>

namespace es {
namespace file {

struct AnimationData {
    struct Meta {
        enum {
            /**
             * ボーンアニメーションに親からのオフセット情報も付与されている
             * FBXのモーション計算がそれに該当する。
             */
                    Flag_BoneHasParentOffset = 0x1 << 0,
        };
        /**
         * 全てのキー情報数
         */
        uint16_t boneKeyNum;

        /**
         * 格納されているボーン数
         */
        uint16_t linkBoneNum;

        /**
         * 最後に打刻されているキーフレーム番号
         */
        uint16_t endFrame;

        /**
         * アニメーションの計算フラグ
         */
        uint16_t flags;

        uint8_t reserve0;
        uint8_t reserve1;
    };

    /**
     * ボーンやモーフごとに管理されるキーフレームの集まり
     */
    struct SymbolTimeline {
        /**
         * 関連付けるボーン名
         */
        uint64_t symbol;

        /**
         * キーフレーム一覧のオフセット位置
         */
        uint16_t offset;

        /**
         * キーフレーム数
         */
        uint16_t num;

        /**
         * このシンボルアニメーションの最大フレーム数
         */
        uint16_t maxFrames;

        uint8_t reserve0;
        uint8_t reserve1;
    };

    struct BoneKeyFrame {
        /**
         * ローカル位置
         */
        vec3 pos;

        /**
         * ローカル回転角
         */
        quat rotate;

        /**
         * 打刻されている時刻
         */
        int16_t frame;

        uint8_t reserve0;
        uint8_t reserve1;
    };

    /**
     * 2つのキーが示すアニメーションが同じである場合trueを返却する
     */
    static bool someFrame(const BoneKeyFrame &a, const BoneKeyFrame &b);

    struct Serialize {
        /**
         * メタ情報
         */
        Meta meta;

        /**
         * 打刻された全てのキーフレーム情報
         */
        std::vector<BoneKeyFrame> boneKeys;

        /**
         * 全てのボーン情報
         */
        std::vector<SymbolTimeline> linkBones;
    };

    Serialize animation;

    static ByteBuffer serialize(Serialize *animation);

    static bool deserialize(Serialize *result, unsafe_array<uint8_t> buffer);
};

}
}


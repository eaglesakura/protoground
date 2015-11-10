#pragma once

#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"
#include <vector>
#include <es/memory/Buffer.hpp>

namespace es {
namespace file {

class SymbolTable;

struct AnimationData {

    /**
     * ファイル拡張子
     * Protoground SKin Animation
     */
    static constexpr char const *FILE_EXT = ".pska";

    /**
     * ファイル拡張子
     * Protoground Animation Slice Json
     */
    static constexpr char const *SLICE_JSON_FILE_EXT = ".pasj";


    PGD_FILE_ALIGN_OBJECT_BEGIN struct Meta {
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

        // 格納されているアニメーションスライス数
        uint16_t sliceNum;

        /**
         * 最後に打刻されているキーフレーム番号
         */
        uint16_t endFrame;


        /**
         * アニメーションの計算フラグ
         */
        uint16_t flags;
    } PGD_FILE_ALIGN_OBJECT_END;

    /**
     * ボーンやモーフごとに管理されるキーフレームの集まり
     */
    PGD_FILE_ALIGN_OBJECT_BEGIN  struct SymbolTimeline {
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

    } PGD_FILE_ALIGN_OBJECT_END;

    PGD_FILE_ALIGN_OBJECT_BEGIN struct BoneKeyFrame {
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
    } PGD_FILE_ALIGN_OBJECT_END;


    // アニメーション分割データ
    PGD_FILE_ALIGN_OBJECT_BEGIN struct Slice {
        // アニメーション名
        uint64_t symbol;

        // 開始フレーム
        uint16_t startFrame;

        // フレーム数
        // 0の場合はstartFrameで固定される
        uint16_t numFrame;

        enum {
            // 終端で停止させる
                    Flag_EndFill = 0x01 << 0,
        };
        uint32_t flags;
    } PGD_FILE_ALIGN_OBJECT_END;

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

        // アニメーション分割情報
        std::vector<Slice> slices;
    };

    Serialize animation;

    /**
     * JSON
     */
    static void parseAnimationSliceJson(const string &json, SymbolTable *symbols, std::vector<Slice> *result);

    static ByteBuffer serialize(Serialize *animation);

    static bool deserialize(Serialize *result, unsafe_array<uint8_t> buffer);
};

}
}


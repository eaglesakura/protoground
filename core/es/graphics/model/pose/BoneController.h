#pragma once

#include "es/protoground.hpp"
#include "es/graphics/model/file/ModelData.h"
#include "es/graphics/model/pose/Transform.h"
#include "AnimationPose.h"

namespace mms {
namespace debug {
class DebugDrawer;
}
}

class btIDebugDraw;

namespace es {

class AnimationPose;

class PhysicsWorld;

class PhysicsController;

struct BoneTable {

    /**
     * 実際に適用すべきボーンテーブル
     * world * invが適用されている
     */
    array_object<mat4> palette;

    /**
     * paletteを転置3x4に配置した行列テーブル
     */
    array_object<Transform> palette34;
};

/**
 * 1つのスキンモデルに紐付いたボーンを管理する。
 */
class BoneController {
public:

    BoneController();

    /**
     * モデルから処理用のデータを生成する
     */
    void initialize(const std::shared_ptr<file::SkinMeshModelData> &model);

    /**
     * 物理コントロールをバインドする
     */
    void bind(std::shared_ptr<PhysicsWorld> &world, std::shared_ptr<PhysicsController> &physics);

    /**
     * ボーン行列を取得する
     */
    unsafe_array<Transform> getBoneTable34() const;

    /**
     * ボーンに対してポーズを適用する
     */
    void update(const AnimationPose *pose);

    /**
     * ボーンに対してポーズを適用する
     * 同時に、物理演算も行う。
     */
    void updateWithPhysics(const AnimationPose *pose, const float deltaTimeSec);

    /**
     * ボーンのポーズをリセットする
     */
    void resetPose();

    const mat4 &getWorld() const;

    /**
     * Actorのワールド座標を指定する
     */
    void setWorld(const mat4 &world);

    ~BoneController() = default;

    /**
    * ボーン情報をデバッグ描画する
    */
    void debugDraw(btIDebugDraw *drawer) const;

    /**
    * ボーン情報をデバッグ描画する
    */
    void debugDraw(const Color fromColor, const Color toColor, btIDebugDraw *drawer) const;

    /**
     * 特定ボーンのみをデバッグ描画する
     */
    void debugDrawSelectBone(const Color fromColor, const Color toColor, const unsigned boneIndex, btIDebugDraw *drawer) const;

    /**
    * 特定ボーンをハイライト描画する
    */
    void debugDrawHighlight(const Color mainColor, const Color otherColor, const unsigned boneIndex, btIDebugDraw *drawer) const;


private:
    /**
     * ワールド行列
     */
    mat4 world;

    /**
     * ワールド行列をFK計算する
     */
    void updateWorldMatrixFKLocal(const AnimationPose *pose);

    /**
     * 行列のIK補正を行う
     */
    void updateWorldMatrixIKLocal();

    /**
     * 計算済みのグローバル行列からボーンに適用べき行列を計算する
     */
    void updateBoneMatrixTable();

    /**
     * ワールド行列を更新する
     */
    mat4 calcWorldMatrix(const unsigned boneIndex) const;

    /**
     * 計算のキャッシュを保持する。
     *
     * この結果は基本的に不変となる。
     */
    struct BoneCache {
        /**
         * ボーン初期座標の逆行列
         */
        mat4 invert;

        /**
         * デフォルトで親からの相対位置を示した行列
         */
        mat4 offsetParent;

        /**
         * 子ボーン一覧
         */
        std::vector<uint16_t> childs;
    };

    std::vector<BoneCache> boneCaches;

    struct BoneMatrix {
        enum CalcPass_e {
            /**
             * ローカル行列計算完了
             */
            CalcPass_Local,

    /**
     * ワールド行列を計算完了
     */
            CalcPass_World,
    /**
     * 未計算
     */
            CalcPass_None,

        };
        CalcPass_e pass = CalcPass_None;

        /**
         * 行列テーブル
         */
        mat4 cache[CalcPass_None];
    };

    /**
     * 原点からのグローバル座標変換テーブル
     * globalTable[i] * inv[i]を適用することで、実際のボーン座標を求めることができる。
     */
    std::vector<BoneMatrix> globalTable;

    sp<BoneTable> boneTable;

    /**
     * 関連付けた物理情報
     */
    sp<PhysicsController> physics;

    /**
     * モデル情報
     */
    sp<file::SkinMeshModelData> model;
};

}




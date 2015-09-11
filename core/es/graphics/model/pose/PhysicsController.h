#pragma once

#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"
#include <vector>
#include <es/graphics/model/file/ModelData.h>

namespace es {

namespace internal {
class RigidBodyImpl;

class ConstraintImpl;
}

class PhysicsWorld;

struct BoneTable;

/**
 * 物理ワールドの制御を行う
 */
class PhysicsController : public virtual Object {
public:
    PhysicsController();

    /**
     * 初期化を行う
     */
    virtual void initialize(const std::shared_ptr<file::SkinMeshModelData> model, const std::shared_ptr<BoneTable> table);

    /**
     * 物理ワールドと関連付ける
     */
    void attachWorld(const std::shared_ptr<PhysicsWorld> &world);

    /**
     * 物理ワールドとの関連を解除する
     */
    void detachWorld();

    /**
     * ボーンの状態を強制的に物理ワールドへ適用する
     *
     * @param forceUpdate trueの場合、物理演算に紐付けられたボーンも強制的にモーション位置に合わせる
     */
    void linkFromBone(bool forceUpdate = false);

    /**
     * 物理ワールドの状態をボーンへ反映させる
     */
    void feedbackToBone();


    const std::shared_ptr<PhysicsWorld> &getWorld() const;

    virtual ~PhysicsController();

private:
    struct Bind {
        /**
         * ボーン番号
         */
        int16_t linkedBoneIndex;

        /**
         * リンクされたボディ
         */
        sp<internal::RigidBodyImpl> body;

        /**
         * 拘束情報
         */
        std::vector<sp<internal::ConstraintImpl> > constraints;
    };

    /**
     * モデルに関連付けた拘束条件一覧
     *
     * シンプルにするため全てのボーンに対してバインドが生成されるが、必要な場合のみrigが生成される。
     */
    std::vector<Bind> binds;

    /**
     * 接続されているワールド情報
     */
    sp<PhysicsWorld> world;

    /**
     * リンクされるモデル情報
     */
    sp<file::SkinMeshModelData> model;

    /**
     * 関連付けられたボーンテーブル
     */
    sp<BoneTable> boneTable;

    friend class internal::ConstraintImpl;

    friend class internal::RigidBodyImpl;
};


}


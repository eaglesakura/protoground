#pragma once

#include "es/protoground.hpp"

/**
 * Bullet Physics
 */
class btDynamicsWorld;

class btCollisionConfiguration;

class btDispatcher;

class btBroadphaseInterface;

class btConstraintSolver;

class btTypedConstraint;

class btRigidBody;

class btIDebugDraw;

namespace es {


/**
 * MMDの物理演算ワールドを管理する
 *
 * アタッチされているrigは、アタッチしている側でメモリ解放を行う必要がある。
 */
class PhysicsWorld {
public:
    PhysicsWorld() = default;

    virtual ~PhysicsWorld() = default;

    /**
     * 設定上の身長と、モデリングされた頂点の高さから、重力情報を設定する。
     * デフォルト値はMMDの初音ミクが設定される。
     */
    virtual void setGravity(const float basicCharacterHeightMeter = 1.58f, const float basicCharacterVertexHeight = 20);

    /**
     * メモリを確保する。
     *
     * セットアップ条件を変えたい場合は適宜オーバーライドしてセットアップする。
     */
    virtual void alloc();

    virtual void attach(btTypedConstraint *constraint);

    virtual void detach(btTypedConstraint *constraint);

    virtual void attach(btRigidBody *object, uint16_t group = 0xFFFF, uint16_t groupMask = 0xFFFF);

    virtual void detach(btRigidBody *object);

    /**
     * 経過時間を指定して物理ワールドを更新する
     */
    virtual void update(const float deltaTimeSec);

    /**
     * ワールドの内容を可視化する
     */
    virtual void debugDrawWorld(btIDebugDraw *debugDrawer);

protected:
    std::shared_ptr<btDynamicsWorld> dynamicsWorld;

    std::shared_ptr<btCollisionConfiguration> configuration;

    std::shared_ptr<btDispatcher> dispatcher;

    std::shared_ptr<btBroadphaseInterface> broadphaseInterface;

    std::shared_ptr<btConstraintSolver> constraintSolver;

    bool allocated = false;

    /**
     * デフォルトは120fps計算
     */
    float fixedTimeStep = 1.0f / 120.0f;

    /**
     * 最大サブステップ数
     *
     * for Mobileでは4もあれば十分では？
     */
    float maxSubStep = 4;
};
}
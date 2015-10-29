#pragma once

#include "es/protoground.hpp"
#include <es/system/Bundle.h>
#include <es/graphics/3d/light/ILight.hpp>
#include <es/memory/BitFlags.hpp>
#include <list>
#include <map>
#include "es/graphics/3d/camera/SceneCamera.h"
#include "es/graphics/3d/actor/SceneActor.h"
#include "es/system/Hash128.h"
#include "es/system/string/HashString.h"

namespace es {

/**
 * 3Dレンダリング用のシーン情報を構築する
 */
class RenderingScene : public Object {
public:
    class SceneListener;

    RenderingScene();

    std::shared_ptr<SceneCamera> getCamera() const;

    /**
     * メインライト（シャドウ生成用のライト）情報を指定する
     */
    void setKeyLight(const std::shared_ptr<IShadowLight> &keyLight);

    const std::shared_ptr<IShadowLight> &getKeyLight() const;

    const Bundle &getOptions() const;

    bool hasActor(const std::shared_ptr<SceneActor> &actor) const;

    bool hasActor(const SceneActor *const actor) const;

    /**
     * メッセージを保留する
     */
    void pendingMessage(const Object *sender, const Hash128 &target, const Bundle &msg);

    /**
     * メッセージを保留する
     */
    void pendingMessage(const Object *sender, const uint32_t groupMask, const Bundle &msg);

    /**
     * メッセージを保留する
     * 全てのオブジェクトに対して送信する
     */
    void pendingMessage(const Object *sender, const Bundle &msg);

    /**
     * 全ての保留メッセージを送信する
     */
    void sendPendingMessages();

    /**
     * 指定したオブジェクトにメッセージを投げる
     */
    void sendMessage(const Object *sender, const Hash128 &target, const Bundle &msg);

    /**
     * 全てのオブジェクトにメッセージを投げる
     */
    void sendMessage(const Object *sender, const uint32_t groupMask, const Bundle &msg);

    /**
     * 管理対象のActorを追加する
     *
     * checkOverwrite:trueの場合、同一オブジェクトが既に登録されているかをチェックする。
     * 通常falseにすることでチェックコストを下げることができる。
     */
    void addActor(const std::shared_ptr<SceneActor> &actor, bool checkOverwrite = false);

    /**
     * 管理対象のActorを排除する
     */
    void removeActor(const std::shared_ptr<SceneActor> &actor);

    /**
     * リスナを追加する
     */
    void addListener(const selection_ptr<SceneListener> &listener);

    /**
     * リスナを削除する
     */
    void removeListener(const selection_ptr<SceneListener> &listener);

    /**
     * 指定されたグループのいずれかに所属している[(groupId & mask) != 0]の場合にコールバックを行う
     */
    void findActors(const uint32_t groupMask, const std::function<void(const RenderingScene *, const std::shared_ptr<SceneActor> &)> &lambda) const;

    /**
     * 管理対象のActorを全てソートする
     */
    void sortActors();


    /**
     * 現在のアップデートIDを取得する。
     * updateBegin()を呼び出す度に更新される。
     *
     * 初期値は0となる。
     */
    uint64_t getUpdateId() const;

    /**
     * 更新を開始する
     */
    virtual void updateBegin();

    /**
     * 更新を行わせる
     */
    virtual void update(const unsigned passId, const float deltaSec);

    /**
     * 更新を終了する
     */
    virtual void updateEnd();

    class SceneListener : public virtual Object {
    public:
        /**
         * 更新を開始した
         */
        virtual void onUpdateBegin(RenderingScene *scene) { }

        /**
         * 更新を行った後に呼び出される
         */
        virtual void onUpdated(RenderingScene *scene, const unsigned passId, const float deltaSec) { }

        /**
         * 更新を完了した
         */
        virtual void onUpdateFinished(RenderingScene *scene) { }
    };

    enum {
        /**
         * Actorの通常アップデートパス
         *
         * これ以外は自由に拡張可能
         */
                PASS_ID_ACTOR_UPDATE = 0x00010000,
    };

    /**
     * メッセージを識別するためのID
     */
    static const HashString MESSAGE_UNIQUE_ID;

    enum MessageUniqueId_e {
        MessageUniqueId_Lyfecycle_surfaceChanged,
        MessageUniqueId_Lyfecycle_onPause,
        MessageUniqueId_Lyfecycle_onResume,
    };

    /**
     * 新しいサーフェイスのサイズを指定する:int32
     */
    static const HashString SURFACE_SIZE_WIDTH;

    /**
     * 新しいサーフェイスのサイズを指定する:int32
     */
    static const HashString SURFACE_SIZE_HEIGHT;

    virtual ~RenderingScene() = default;

    static Bundle makeSurfaceChangeMessage(const uint32_t width, const uint32_t height);

protected:
    /**
     * メッセージのハンドリングを行う
     */
    virtual void onSendMessage(const Object *sender, const Bundle &msg) { }

private:
    /**
     * Actorの管理情報
     */
    class ActorCache;

    struct PendingMessage {
        /**
         * 送信者
         */
        Object *sender = nullptr;

        /**
         * 送信メッセージ
         */
        Bundle opt;

        /**
         * 送信対象をマスクする。
         * 0の場合は指定されたハッシュに投げる
         */
        uint32_t groupMask = 0;

        /**
         * ターゲットを固定する
         */
		Hash128 target;
    };

    /**
     * レンダリング用のメインカメラ
     */
    sp<SceneCamera> camera;

    /**
     * シャドウ生成用のキーライトを指定する
     */
    sp<IShadowLight> keyLight;

    /**
     * 管理対象のアクター一覧
     */
    std::list<sp<ActorCache> > actors;

    /**
     * ハッシュごとに管理されたアクター
     */
    std::map<Hash128, sp<ActorCache>> hashActors;

    /**
     * 保留されている送信メッセージ
     */
    std::vector<PendingMessage> pendingMessages;

    /**
     * シーン全体のリスナ
     */
    std::vector<selp<SceneListener> > listeners;

    /**
     * 各種オプション情報
     */
    Bundle options;

    /**
     * 現在のアップデートIDを収得する。
     * updateBegin()を呼び出す度に更新される。
     */
    uint64_t updateId = 0;
};

}

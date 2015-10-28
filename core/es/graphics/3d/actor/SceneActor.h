#pragma once

#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"
#include "es/system/Bundle.h"
#include <list>

namespace es {

class RenderingScene;

class AsyncGraphicsRenderer;

class IDevice;

/**
 * シーン上の存在するオブジェクトを管理する
 *
 * レンダリングを行わないオブジェクトもActorとして管理される。
 */
class SceneActor : public virtual Object {
public:
    SceneActor();

    SceneActor(const long_hash &hash);

    /**
     * シーンに登録された
     */
    virtual void onRegister(RenderingScene *scene, std::weak_ptr<SceneActor> self);

    /**
     * シーンから排除された
     */
    virtual void onUnregister();


    /**
     * シーン情報の参照を取得する。
     *
     * この参照は通常のポインタであるため、所有権に注意する。
     */
    RenderingScene *getRenderingSceneRef() const;

    class ActorListener;

    class ActorRenderer;

    /**
     * 属性フラグ情報
     */
    enum Attribute_e {
        /**
         * 半透明属性を持つ場合true
         *
         * ソート時の目安として利用する
         */
                Attribute_Translucency = 0x1 << 0,

        /**
         * ソートを行わず、適当な順序でのレンダリングを許す
         */
                Attribute_Ignore_Sort = 0x1 << 1,

        /**
         * レンダリング対象とならない。
         */
                Attribute_Ignore_Rendering = 0x1 << 2 | Attribute_Ignore_Sort,
    };

    /**
     * 検索用のハッシュを取得する
     * 標準では自身のポインタを基準とするが、任意で書き換えを行える
     */
    const long_hash &getHash() const;

    /**
     * センタリング座標を取得する。
     *
     * vec3は必ずfloat[3]以上の要素を持つ。
     * XYZの順序で書き込む必要がある。
     * 要素を正常に書き込んだ場合はtrueを返し、正常に返せない場合はfalseを返す。
     */
    virtual bool getCenter(float *vec3) const = 0;

    /**
     * 無効なオブジェクトであればtrueを返却する
     *
     * updateEndのタイミングでシーンから削除される。
     * 実際にシーンから削除されるまではmessageが到達する。
     */
    virtual bool isInvalidate() const = 0;

    /**
     * 所属グループフラグを取得する
     *
     * 32グループ（もしくは未所属）に指定できる。
     */
    uint32_t getGroupFlags() const;

    /**
     * 属性情報を取得する
     *
     * Attribute_eをOR指定する。
     */
    uint32_t getAttributeFlags() const;

    /**
     * アップデートを行わせる
     *
     * アップデートは複数回行う場合があるので、それぞれアップデートパスを指定できるようにする。
     *
     * @param passId   パスを識別するためのUID
     * @param deltaSec 前のフレームからの差分時間。ただし、現実時間とは調整が行われる場合がある。
     */
    virtual void update(RenderingScene *scene, const uint passId, const float deltaSec);

    /**
     * メッセージングを行う
     *
     * @param scene  管理されているシーン
     * @param sender メッセージを送ったオブジェクト
     * @param arg    メッセージ本体
     */
    virtual void message(RenderingScene *scene, const Object *sender, const Bundle &msg);

    /**
     * レンダリング用パケットを生成する
     */
    virtual void makeRenderingPacket(const RenderingScene *scene, AsyncGraphicsRenderer *renderer, const uint globalPass);

    /**
     * リスナを追加する
     * 重複チェックは行わない。
     */
    void addListener(selection_ptr<ActorListener> listener);

    /**
     * リスナを削除する
     * 複数登録されている場合は全て削除される
     */
    void removeListener(selection_ptr<ActorListener> listener);

    /**
     * レンダラを登録する
     * 重複チェックは行わない。
     */
    void addRenderer(selection_ptr<ActorRenderer> renderer);

    /**
     * レンダラを削除する
     * 複数登録されている場合は全て削除される
     */
    void removeRenderer(selection_ptr<ActorRenderer> renderer);

    /**
     * Actorに到達したメッセージのハンドリングを行う
     */
    class ActorListener : public virtual Object {
    public:
        virtual void onRegister(SceneActor *act, std::weak_ptr<ActorListener> self) { };

        virtual void onUnregister() { };

        /**
         * アップデートを行う
         */
        virtual void onUpdate(RenderingScene *scene, SceneActor *actor, const uint passId, const float deltaSec) { }

        /**
         * メッセージ受信ハンドリングを行う
         */
        virtual void onReceivedMessage(RenderingScene *scene, SceneActor *actor, const Object *sender, const Bundle &msg) { }

        ActorListener() = default;

        virtual ~ActorListener() = default;
    };

    /**
     * レンダリングを行う
     *
     * レンダラはGraphics Engineに結びつくため、別途用意する
     */
    class ActorRenderer : public virtual Object {
    public:
        virtual void onRegister(SceneActor *act, std::weak_ptr<ActorRenderer> self) { };

        virtual void onUnregister() { };

        /**
         * レンダリングパケットを生成させる。
         *
         * メインスレッドから呼び出され、生成されたパケットはレンダリングスレッドで利用される。
         * そのため、パケット内部で破壊的な変更を行うと正常に動作しない。
         *
         * @param パケットを登録した場合trueを返却する。
         *        trueを返却した場合、AsyncGraphicsRendererでパケットがclearされるまで、オブジェクトの生存を保証する。
         */
        virtual bool onMakeRenderingPacket(const RenderingScene *scene, const SceneActor *actor, AsyncGraphicsRenderer *renderer, const uint globalPass) = 0;

        virtual ~ActorRenderer() = default;
    };


    virtual ~SceneActor() = default;

protected:
    virtual void onMessageLyfecycleSurfaceChanged(RenderingScene *scene, const Object *sender, const Bundle &msg, const int32_t width, const int32_t height) { }

    virtual void onMessageLyfecyclePause(RenderingScene *scene, const Object *sender, const Bundle &msg) { }

    virtual void onMessageLyfecycleResume(RenderingScene *scene, const Object *sender, const Bundle &msg) { }

    /**
     * 所属グループフラグ
     *
     * デフォルトは1番のみに所属している。
     */
    uint groupFlags = 0x01;

    /**
     * 属性フラグ
     *
     * デフォルトは追加属性を持たない
     */
    uint attributeFlags = 0;

    /**
     * シーンをキャストして取得する
     */
    template<typename T>
    T *getSceneQuery() const {
        return T::query::from(renderingSceneRef);
    }

private:
    /**
     * 検索用ハッシュ
     */
    long_hash hash;

    /**
     * 登録されているリスナ一覧
     */
    std::list<selp<ActorListener> > listeners;

    /**
     * 登録されているレンダラ一覧
     */
    std::list<selp<ActorRenderer> > renderers;

    /**
     * 接続されているシーン
     */
    RenderingScene *renderingSceneRef = nullptr;

    wp<SceneActor> self;
};

}
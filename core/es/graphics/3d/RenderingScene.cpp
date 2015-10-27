#include <es/system/string/HashStringTable.h>
#include "RenderingScene.h"
#include "es/internal/protoground-internal.hpp"

namespace es {

ES_GLOBAL_HASH_IMPL(RenderingScene, MESSAGE_UNIQUE_ID);
ES_GLOBAL_HASH_IMPL(RenderingScene, SURFACE_SIZE_WIDTH);
ES_GLOBAL_HASH_IMPL(RenderingScene, SURFACE_SIZE_HEIGHT);


class RenderingScene::ActorCache {
public:
    enum {
        /**
         * 不透明オブジェクト
         * 手前から後方に向かって描画する
         */
                PRIORITY_OPAQUE = 1,

        /**
         * 半透明オブジェクト
         * 後方から手前に向かって描画する
         */
                PRIORITY_TRANSLUCENT = 2,

        /**
         * ソート対象ではない
         */
                PRIORITY_NO_SORT = 100,
    };

    /**
     * ソート用の重心位置
     */
    vec3 center;

    /**
     * カメラからみたセンター位置
     */
    vec3 lookCenter;

    int sortPriority = PRIORITY_NO_SORT;

    /**
     * ソート後の描画優先順位
     */
    int sortedIndex = -1;
    sp<SceneActor> actor;

    ActorCache(const sp<SceneActor> &actor) : actor(actor) {
    }

    bool isDelete() {
        return actor->isInvalidate();
    }

    void update(RenderingScene *scene, const uint passId, const float deltaSec) {
        actor->update(scene, passId, deltaSec);
    }

    /**
     * ソート用のキャッシュ情報を更新する
     */
    void updateSortCache(const mat4 &look) {
        if (!isSortTarget()) {
            return;
        }

        if (actor->getAttributeFlags() & SceneActor::Attribute_Translucency) {
            // 半透明を持つなら、非優先的に描画する
            sortPriority = PRIORITY_TRANSLUCENT;
        } else {
            // 不透明として優先的に描画する
            sortPriority = PRIORITY_OPAQUE;
        }

        // 位置を取得する
        if (!actor->getCenter((float *) &center)) {
            // 位置を取得できなかったので何もしない
            return;
        }

        // 位置を更新する
        util::multiply(&lookCenter, look, center);
    }

    /**
     * ソート対象であればtrue
     */
    bool isSortTarget() {
        return (actor->getAttributeFlags() & SceneActor::Attribute_Ignore_Sort) == 0;
    }

    static int compare(const ActorCache *lhs, const ActorCache *rhs) {
        const auto lhs_sortPriority = lhs->sortPriority;
        const auto rhs_sortPriority = rhs->sortPriority;

        if (lhs_sortPriority == PRIORITY_NO_SORT || rhs_sortPriority == PRIORITY_NO_SORT) {
            // 片方がソート対象ではない場合、これは何も行わない
            return 0;
        }
        if (lhs_sortPriority != rhs_sortPriority) {
            // プライオリティが違う場合は優先順に従ってソートする
            return (lhs_sortPriority - rhs_sortPriority);
        } else {
            // Zソート順を確定させる
            if (lhs_sortPriority == PRIORITY_TRANSLUCENT) {
                // 半透明はZが大きいものを手前にする
                return (lhs->lookCenter.z > rhs->lookCenter.z) ? -1 : 1;
            } else {
                // 不透明はZが小さいものを手前にする
                return (lhs->lookCenter.z < rhs->lookCenter.z) ? -1 : 1;
            }
        }
    }
};


RenderingScene::RenderingScene() {
    camera.reset(new SceneCamera());
}

std::shared_ptr<SceneCamera> RenderingScene::getCamera() const {
    return camera;
}

const std::shared_ptr<IShadowLight> &RenderingScene::getKeyLight() const {
    return keyLight;
}

const Bundle &RenderingScene::getOptions() const {
    return options;
}

bool RenderingScene::hasActor(const std::shared_ptr<SceneActor> &actor) const {
    return hasActor(actor.get());
}

bool RenderingScene::hasActor(const SceneActor *const actor) const {
    if (!actor) {
        return false;
    }
    // Actorのハッシュから検索する
    return hashActors.find(actor->getHash()) != hashActors.end();
}

void RenderingScene::addActor(const std::shared_ptr<SceneActor> &actor, bool checkOverwrite) {
    assert(actor);

    if (checkOverwrite) {
        if (hasActor(actor)) {
            eslog("hasActor(%x) abort...", actor.get());
            return;
        }
    }

    // actorを登録する
    sp<ActorCache> cache(new ActorCache(actor));
    actors.push_back(cache);
    hashActors.insert(std::pair<long_hash, sp<ActorCache> >(actor->getHash(), cache));

    actor->onRegister(this, actor);
}

void RenderingScene::removeActor(const std::shared_ptr<SceneActor> &actor) {
    // mappingを削除する
    hashActors.erase(actor->getHash());

    // リストを削除する
    auto itr = actors.begin();
    const auto end = actors.end();
    while (itr != end) {
        if ((*itr)->actor == actor) {
            (*itr)->actor->onUnregister();
            actors.erase(itr);
            return;
        }

        ++itr;
    }
}

void RenderingScene::setKeyLight(const std::shared_ptr<IShadowLight> &keyLight) {
    RenderingScene::keyLight = keyLight;
}

void RenderingScene::sortActors() {

    mat4 look = camera->calcLookMatrix();

    // 事前計算を行う
    for (const auto &actor : actors) {
        actor->updateSortCache(look);
    }

    // ソートを行う
    actors.sort([](const sp<ActorCache> &lhs, const sp<ActorCache> &rhs) -> int {
        return ActorCache::compare(lhs.get(), rhs.get());
    });

    // 順序を到達させる
    int index = 0;
    for (const auto &actor : actors) {
        actor->sortedIndex = index;
        ++index;
    }
}

void RenderingScene::findActors(const uint32_t groupMask, const std::function<void(const RenderingScene *, const std::shared_ptr<SceneActor> &)> &lambda) const {
    for (const auto &actor : actors) {
        if (actor->actor->getGroupFlags() & groupMask) {
            lambda(this, actor->actor);
        }
    }
}

void RenderingScene::updateBegin() {
    ++updateId;

    auto itr = listeners.begin();
    while (itr != listeners.end()) {
        auto item = itr->lock();
        if (!item) {
            itr = listeners.erase(itr);
        } else {
            item->onUpdateBegin(this);
            ++itr;
        }
    }
}


void RenderingScene::update(const uint passId, const float deltaSec) {
    for (auto &actor : actors) {
        actor->update(this, passId, deltaSec);
    }

    // 保留されているメッセージを全て吐き出す
    while (!pendingMessages.empty()) {
        sendPendingMessages();
    }

    // リスナに通知
    {
        auto itr = listeners.begin();
        while (itr != listeners.end()) {
            auto item = itr->lock();
            if (!item) {
                itr = listeners.erase(itr);
            } else {
                item->onUpdated(this, passId, deltaSec);
                ++itr;
            }
        }
    }
}

void RenderingScene::updateEnd() {

    auto itr = actors.begin();
    auto end = actors.end();
    while (itr != end) {
        if ((*itr)->isDelete()) {
            hashActors.erase((*itr)->actor->getHash());

            itr = actors.erase(itr);
            end = actors.end();
        } else {
            ++itr;
        }
    }

    // リスナに通知
    {
        auto itr = listeners.begin();
        while (itr != listeners.end()) {
            auto item = itr->lock();
            if (!item) {
                itr = listeners.erase(itr);
            } else {
                item->onUpdateFinished(this);
                ++itr;
            }
        }
    }
}

void RenderingScene::pendingMessage(const Object *sender, const long_hash &target, const Bundle &msg) {
    PendingMessage pm;
    pm.opt = msg;
    pm.sender = (Object *) sender;
    pm.target = target;

    pendingMessages.push_back(pm);
}

void RenderingScene::pendingMessage(const Object *sender, const uint32_t groupMask, const Bundle &msg) {
    PendingMessage pm;
    pm.opt = msg;
    pm.sender = (Object *) sender;
    pm.groupMask = groupMask;

    pendingMessages.push_back(pm);
}

void RenderingScene::pendingMessage(const Object *sender, const Bundle &msg) {
    pendingMessage(sender, 0xFFFFFFFF, msg);
}

void RenderingScene::sendPendingMessages() {
    auto msgCache = pendingMessages;
    pendingMessages.clear();

    for (const auto &msg : msgCache) {
        if (msg.groupMask) {
            sendMessage(msg.sender, msg.groupMask, msg.opt);
        } else {
            sendMessage(msg.sender, msg.target, msg.opt);
        }
    }
}

void RenderingScene::sendMessage(const Object *sender, const long_hash &target, const Bundle &msg) {
    // 指定したオブジェクトに投げる
    auto itr = hashActors.find(target);
    if (itr != hashActors.end()) {
        itr->second->actor->message(this, sender, msg);
    }

    onSendMessage(sender, msg);
}

void RenderingScene::sendMessage(const Object *sender, const uint32_t groupMask, const Bundle &msg) {
    for (auto act : actors) {
        if (act->actor->getGroupFlags() & groupMask) {
            act->actor->message(this, sender, msg);
        }
    }

    onSendMessage(sender, msg);
}

void RenderingScene::addListener(const selection_ptr<SceneListener> &listener) {
    listeners.push_back(listener);
}

void RenderingScene::removeListener(const selection_ptr<SceneListener> &listener) {
    auto itr = listeners.begin();
    while (itr != listeners.end()) {
        if (*itr == listener) {
            itr = listeners.erase(itr);
        } else {
            ++itr;
        }
    }
}

uint64_t RenderingScene::getUpdateId() const {
    return updateId;
}

Bundle RenderingScene::makeSurfaceChangeMessage(const uint32_t width, const uint32_t height) {
    Bundle msg;
    msg.putInt32(MESSAGE_UNIQUE_ID, MessageUniqueId_Lyfecycle_surfaceChanged);
    msg.putInt32(SURFACE_SIZE_WIDTH, width);
    msg.putInt32(SURFACE_SIZE_HEIGHT, height);
    return msg;
}
}
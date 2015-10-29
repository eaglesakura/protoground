#include "SceneActor.h"
#include "es/graphics/3d/RenderingScene.h"
#include "es/graphics/rendering/AsyncGraphicsRenderer.h"

namespace es {

SceneActor::SceneActor() {
    hash = Hash128::from((void *) this);
}

SceneActor::SceneActor(const Hash128 &hash) : hash(hash) {
}

const Hash128 &SceneActor::getHash() const {
    return hash;
}

uint32_t SceneActor::getGroupFlags() const {
    return groupFlags;
}

uint32_t SceneActor::getAttributeFlags() const {
    return attributeFlags;
}

void SceneActor::update(RenderingScene *scene, const unsigned passId, const float deltaSec) {
    // 更新を伝える
    for (auto &listener : listeners) {
        listener->onUpdate(scene, this, passId, deltaSec);
    }
}

void SceneActor::makeRenderingPacket(const RenderingScene *scene, AsyncGraphicsRenderer *gr, const unsigned globalPass) {
    for (auto &renderer : renderers) {
        if (renderer->onMakeRenderingPacket(scene, this, gr, globalPass)) {
            gr->addSaveObject(renderer.lock());
        }
    }
}

void SceneActor::message(RenderingScene *scene, const Object *sender, const Bundle &arg) {
    // メッセージを伝える
    for (auto &listener : listeners) {
        listener->onReceivedMessage(scene, this, sender, arg);
    }

    int32_t uid = arg.getInt32(RenderingScene::MESSAGE_UNIQUE_ID, 0xFEFE0000);
    if (uid == RenderingScene::MessageUniqueId_Lyfecycle_surfaceChanged) {
        int32_t width = arg.getInt32(RenderingScene::SURFACE_SIZE_WIDTH, -1);
        int32_t height = arg.getInt32(RenderingScene::SURFACE_SIZE_HEIGHT, -1);
        if (width > 0 && height > 0) {
            onMessageLyfecycleSurfaceChanged(scene, sender, arg, width, height);
        }
    } else if (uid == RenderingScene::MessageUniqueId_Lyfecycle_onPause) {
        onMessageLyfecyclePause(scene, sender, arg);
    } else if (uid == RenderingScene::MessageUniqueId_Lyfecycle_onResume) {
        onMessageLyfecycleResume(scene, sender, arg);
    }
}

void SceneActor::addListener(selection_ptr<ActorListener> listener) {
    listeners.push_back(listener);
    listener->onRegister(this, listener.lock());
}

void SceneActor::removeListener(selection_ptr<ActorListener> listener) {
    // 一致したリスナを削除する
    auto itr = listeners.begin();
    auto end = listeners.end();
    while (itr != end) {
        if ((*itr) == listener) {
            (*itr)->onUnregister(this, (*itr).lock());
            itr = listeners.erase(itr);
            end = listeners.end();
        } else {
            ++itr;
        }
    }
}

void SceneActor::addRenderer(selection_ptr<ActorRenderer> renderer) {
    renderers.push_back(renderer);
    renderer->onRegister(this, renderer.lock());
}

void SceneActor::removeRenderer(selection_ptr<ActorRenderer> renderer) {
    // 一致したリスナを削除する
    auto itr = renderers.begin();
    auto end = renderers.end();
    while (itr != end) {
        if ((*itr) == renderer) {
            (*itr)->onUnregister(this, (*itr).lock());
            itr = renderers.erase(itr);
            end = renderers.end();
        } else {
            ++itr;
        }
    }
}

void SceneActor::onRegister(RenderingScene *scene, std::weak_ptr<SceneActor> self) {
    this->renderingSceneRef = scene;
    this->self = self;
}

void SceneActor::onUnregister() {
    this->renderingSceneRef = nullptr;
    this->self.reset();
}

RenderingScene *SceneActor::getRenderingSceneRef() const {
    return renderingSceneRef;
}

}
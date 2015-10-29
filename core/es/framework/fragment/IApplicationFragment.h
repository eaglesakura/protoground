#pragma once

#include    "es/protoground.hpp"

namespace es {

class ApplicationFragmentController;

class IApplication;

/**
 * アプリの子要素としてコントローラーを追加する
 */
class IApplicationFragment : public virtual Object {
    ApplicationFragmentController *controller;

    int64_t id;

    std::string tag;

protected:
    IApplicationFragment() {
        controller = nullptr;
        id = (int64_t) this;
    }

public:
    virtual ~IApplicationFragment() {
    }

    virtual void onAttach(ApplicationFragmentController *ctrl) {
        this->controller = ctrl;
    }

    /**
     * サーフェイスサイズが変更された
     */
    virtual void onSurfaceResized(const unsigned width, const unsigned height) {
    }

    /**
     * フラグメント復旧を行う
     */
    virtual void onResume() {
    }

    /**
     * 毎フレーム更新を行う
     */
    virtual void onUpdate() {
    }

    /**
     * フラグメント休止を行う
     */
    virtual void onPause() {
    }

    /**
     * 解放処理を行う
     */
    virtual void onDestroy() {
    }

    virtual void onDetatch() {
        this->controller = NULL;
    }

    /**
     * アタッチされているアプリを取得する
     */
    virtual IApplication *getApplication() const;

    /**
     * フラグメント用コントローラーを取得する
     */
    virtual ApplicationFragmentController *getController() const {
        return controller;
    }

    virtual uint64_t getId() const {
        return id;
    }

    virtual const std::string &getTag() const {
        return tag;
    }

};

}


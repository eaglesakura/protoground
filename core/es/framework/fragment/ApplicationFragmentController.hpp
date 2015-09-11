#pragma once

#include    "IApplicationFragment.h"
#include    "es/system/Thread.hpp"
#include    <vector>

namespace es {

/**
 * フラグメント管理を行う
 */
class ApplicationFragmentController : public virtual Object {
public:
    ApplicationFragmentController(IApplication *app) {
        assert(app);
        this->application = app;
    }

    virtual ~ApplicationFragmentController() {
    }

    /**
     * フラグメント追加を行う
     */
    virtual void addFragment(selection_ptr<IApplicationFragment> fragment) {
        mutex_lock lock(transactionLock);

        requestAddFragment.push_back(fragment);
    }

    /**
     * フラグメントの削除を行う
     */
    virtual void removeFragment(selection_ptr<IApplicationFragment> fragment) {
        mutex_lock lock(transactionLock);

        requestRemoveFragment.push_back(fragment);

    }

    /**
     * 追加・削除リクエストの処理を行う
     */
    virtual void commit() {
        mutex_lock lock(transactionLock);

        // 追加リストを処理する
        if (!requestAddFragment.empty()) {
            auto itr = requestAddFragment.begin(), end = requestAddFragment.end();
            while (itr != end) {
                onAddFragment(*itr);
                ++itr;
            }

            requestAddFragment.clear();
        }

        // 削除リストを処理する
        if (!requestRemoveFragment.empty()) {
            auto itr = requestRemoveFragment.begin(), end = requestRemoveFragment.end();
            while (itr != end) {
                onRemoveFragment(*itr);
                ++itr;
            }

            requestRemoveFragment.clear();
        }
    }

    /**
     * フラグメント復旧を行う
     */
    virtual void onSurfaceResized(const uint width, const uint height) {
        auto itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onSurfaceResized(width, height);
            ++itr;
        }
    }

    /**
     * フラグメント復旧を行う
     */
    virtual void onResume() {
        auto itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onResume();
            ++itr;
        }
    }

    /**
     * 毎フレーム更新を行う
     */
    virtual void onUpdate() {
        auto itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onUpdate();
            ++itr;
        }

    }

    /**
     * フラグメント休止を行う
     */
    virtual void onPause() {
        auto itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onPause();
            ++itr;
        }
    }

    /**
     * 解放処理を行う
     */
    virtual void onDestroy() {
        auto itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onDestroy();
            ++itr;
        }
    }

    virtual IApplication *getApplication() const {
        return application;
    }

    /**
     * IDを指定してFragmentを取得する
     */
    virtual selection_ptr<IApplicationFragment> findFragmentById(const uint64_t id) const {
        mutex_lock lock(transactionLock);

        auto itr = fragments.begin(), end = fragments.end();
        while (itr != end) {
            if ((*itr)->getId() == id) {
                return (*itr);
            }
            ++itr;
        }
        return selection_ptr<IApplicationFragment>();
    }

    /**
     * タグを指定してFragmentを取得する
     */
    virtual selection_ptr<IApplicationFragment> findFragmentByTag(const std::string &tag) {
        mutex_lock lock(transactionLock);

        auto itr = fragments.begin(), end = fragments.end();
        while (itr != end) {
            if ((*itr)->getTag() == tag) {
                return (*itr);
            }
            ++itr;
        }
        return selection_ptr<IApplicationFragment>();
    }

protected:
    virtual void onAddFragment(selection_ptr<IApplicationFragment> fragment) {
        assert(fragment);

        fragment->onAttach(this);
        fragment->onResume();

        fragments.push_back(fragment);
    }

    virtual void onRemoveFragment(selection_ptr<IApplicationFragment> fragment) {
        assert(fragment);

        fragment->onPause();
        fragment->onDetatch();

        auto itr = fragments.begin(), end = fragments.end();
        while (itr != end) {
            if ((*itr) == fragment) {
                fragments.erase(itr);
                return;
            }
            ++itr;
        }
    }

private:
    typedef typename std::vector<selection_ptr<IApplicationFragment> > fragment_container;

    mutable mutex transactionLock;

    /**
     * 管理中のフラグメント
     */
    fragment_container fragments;

    /**
     * フラグメントの追加リクエスト
     */
    fragment_container requestAddFragment;

    /**
     * フラグメントの削除リクエスト
     */
    fragment_container requestRemoveFragment;

    IApplication *application;
};

}

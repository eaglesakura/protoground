#pragma once

#include    "es/protoground.hpp"

namespace es {

namespace internal {

/**
 * 何も行わないDeleterを用意する
 */
void dummy_deleter(void *p);

}

namespace util {

#if 0
/*
 * スマートポインタのダウンキャストを行う
 */
template<typename T, typename T2>
::std::shared_ptr<T> downcast(const ::std::shared_ptr<T2> &obj) {
    if (!obj) {
        return ::std::shared_ptr<T>();
    }

    try {
        return ::std::dynamic_pointer_cast<T>(obj);
    } catch (...) {
        return ::std::shared_ptr<T>();
    }
}
#endif


/**
 * pをshared_ptrにラップする。
 * ただし、delete権限を持たないため削除は別途行わなければならない。
 * また、先行して削除されるとpは無効となることに注意する。
 */
template<typename T>
std::shared_ptr<T> wrapSharedPtr(const T *p) {
    return std::shared_ptr<T>((T *) p, internal::dummy_deleter);
}

}

/**
 * 通常のポインタ、スマートポインタ、ウィークポインタのいずれかを選んで設定する。
 * jc_sp/jc_wpを利用する。
 * 生ポインタは、例えば最上位クラス等、オブジェクトの生存を設計上完全にコントロール可能な場合に利用する。
 *
 * 利用する側は通常のポインタのように振る舞う。
 * 例えば、ライブラリ内のリスナポインタ保持等、個々のアプリごとに設計思想が違う場合に利用する。
 */
template<typename T>
class selection_ptr {
public:
    selection_ptr() {
    }

    /**
     * コピーコンストラクタ
     */
    selection_ptr(const selection_ptr &origin) {
        this->shared = origin.shared;
        this->weak = origin.weak;
    }

    /**
     * 生ポインタで生成
     */
    template<typename T2>
    selection_ptr(const T2 *p) {
        this->shared = util::wrapSharedPtr(p);
    }

    /**
     *
     */
    template<typename T2>
    selection_ptr(const ::std::shared_ptr<T2> &shared) {
        this->shared = shared;
    }

    /**
     *
     */
    template<typename T2>
    selection_ptr(const std::weak_ptr<T2> &weak) {
        this->weak = weak;
    }

    /**
     * ポインタが有効であればtrueを返す
     */
    bool exist() const {
        return (bool) shared || !weak.expired();
    }

    /**
     * ポインタをロックする。
     *
     * Rawポインタはロック不可能なので、何も行わないDeleterでラップすることで実装を共通化する。
     */
    std::shared_ptr<T> lock() const {
        if (shared) {
            return shared;
        } else {
            return weak.lock();
        }
    }

    /**
     * 生ポインタを取得する
     */
    T *get() {
        if (shared) {
            return shared.get();
        } else {
            return weak.lock().get();
        }
    }

    /**
     * 生ポインタを取得する
     */
    T *get() const {
        if (shared) {
            return shared.get();
        } else {
            return weak.lock().get();
        }
    }

    /**
     * アロー演算子
     */
    T *operator->() {
        T *result = get();
        assert(result != NULL);
        return result;
    }

    /**
     * アロー演算子
     */
    const T *operator->() const {
        T *result = get();
        assert(result != NULL);
        return result;
    }

    /**
     * ポインタをリセットする
     */
    void reset() {
        weak.reset();
        shared.reset();
    }

    /**
     * ポインタをリセットする
     */
    void reset(const T *p) {
        reset();
    }

    /**
     * ポインタをリセットする
     */
    void reset(const ::std::shared_ptr<T> &p) {
        reset();
        shared = p;
    }

    /**
     * ポインタをリセットする
     */
    void reset(const std::weak_ptr<T> &p) {
        reset();
        weak = p;
    }

#if 0
    /**
     * 特定の型へダウンキャストする
     */
    template<typename DC>
    selection_ptr<DC> downcast() const {
        selection_ptr<DC> result;

        if (shared) {
            result.shared = es::util::downcast<DC>(shared);
        } else {
            result.weak = es::util::downcast<DC>(weak.lock());
        }

        return result;
    }
#endif

    /**
     * 同値である場合はtrue
     */
    bool operator==(const selection_ptr &p) const {
        return lock() == p.lock();
    }

    /**
     * 異なる値である場合はtrue
     */
    bool operator!=(const selection_ptr &p) const {
        return lock() != p.lock();
    }

    operator bool() const {
        return exist();
    }

    bool operator!() const {
        return !exist();
    }

    selection_ptr<T> &operator=(const selection_ptr<T> &cpy) {
        this->shared = cpy.shared;
        this->weak = cpy.weak;
        return (*this);
    }

private:
    /**
     *
     */
    ::std::shared_ptr<T> shared;

    /**
     *
     */
    ::std::weak_ptr<T> weak;
};

}

#pragma once

#include "es/protoground.hpp"

namespace es {

/**
 * Objectのクエリ結果を保持して、dynamic_cast相当の挙動を行わせる
 * 本質的に型安全ではないため、template引数には注意をすること。
 *
 * ポインタを直接指定した場合はダミーのDeleterを利用することで削除を行わなくする。
 */
template<typename T, uint64_t InterfaceId>
class query_ptr {
public:
    static T *from(const Object *base) {
        T *p = nullptr;
        if (base && base->queryInterface(InterfaceId, (void **) &p) == Object::QueryResult_Success) {
            assert(p);
            return p;
        } else {
            return nullptr;
        }
    }

    query_ptr(const std::shared_ptr<Object> &base) {
        reset(base);
    }

    query_ptr(const Object *base) {
        reset(base);
    }

    query_ptr(const std::shared_ptr<T> &base) {
        this->base = base;
        this->p = base.get();
    }

    query_ptr(const query_ptr<T, InterfaceId> &origin) {
        this->base = origin.base;
        this->p = origin.p;
    }

    query_ptr() {
    }

    /**
     * 型安全な場合のみが許される
     */
    query_ptr<T, InterfaceId> &operator=(const query_ptr<T, InterfaceId> &org) {
        base = org.base;
        p = org.p;
        return *this;
    }

    void reset(const std::shared_ptr<Object> &base) {
        this->p = nullptr;
        this->base = base;

        if (base && (base->queryInterface(InterfaceId, (void **) &p) == Object::QueryResult_Success)) {
            // queryに成功した場合は常にポインタが有効でなければならない
            assert(p);
        }
    }

    void reset(const Object *base) {
        this->p = nullptr;
        this->base = util::wrapSharedPtr(base);

        if (base->queryInterface(InterfaceId, (void **) &p) == Object::QueryResult_Success) {
            // queryに成功した場合は常にポインタが有効でなければならない
            assert(p);
        }
    }

    bool operator==(const query_ptr<T, InterfaceId> &check) const {
        return base == check.base;
    }

    bool operator!=(const query_ptr<T, InterfaceId> &check) const {
        return base != check.base;
    }

    operator bool() const {
        return p != nullptr;
    }

    bool operator!() const {
        return p == nullptr;
    }

    T *operator->() const {
        return p;
    }

    T *operator->() {
        return p;
    }

    T *get() const {
        return p;
    }

    T &operator*() {
        assert(p);
        return *p;
    }

    const T &operator*() const {
        assert(p);
        return *p;
    }

    /**
     * クエリ前の元オブジェクトを取得する
     */
    std::shared_ptr<Object> getBase() const {
        return base;
    }

private:

    sp<Object> base;
    T *p = nullptr;
};

}
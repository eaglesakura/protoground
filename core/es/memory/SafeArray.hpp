#pragma once

#include    "es/protoground.hpp"
#include    <string.h>
#include    <vector>

namespace es {

namespace util {

template<typename T>
void safe_delete(T **p) {
    if (*p) {
        delete (*p);
        (*p) = nullptr;
    }
}

template<typename T>
void safe_delete_array(T **p) {
    if (*p) {
        delete[] (*p);
        (*p) = nullptr;
    }
}
}

/**
 * safe_arrayから一部を切り取ったクラス
 * ポインタの削除は行わないため、利用する箇所に注意すること
 */
template<typename value_type>
struct unsafe_array {
    value_type *ptr;
    int length;

    unsafe_array() {
        ptr = nullptr;
        length = 0;
    }

    template<typename value_type2>
    unsafe_array(const value_type2 *p, const int length) {
        this->ptr = (value_type *) p;
        this->length = length;
    }

    template<typename value_type2>
    unsafe_array(const unsafe_array<value_type2> &cpy) {
        this->ptr = (value_type *) cpy.ptr;
        this->length = cpy.length;
    }

    /**
     * オペレータアクセスを提供する
     */
    inline value_type &operator[](const int index) {
        assert(index < length);
        return ptr[index];
    }

    /**
     * オペレータアクセスを提供する
     */
    inline const value_type &operator[](const int index) const {
        assert(index < length);
        return ptr[index];
    }

    inline unsafe_array<value_type> &operator+=(int num) {
        assert(length >= num);
        ptr += num;
        length -= num;

        if (!length) {
            ptr = nullptr;
        }
        return *this;
    }

    /**
     * ポインタを次へ移動する
     */
    inline unsafe_array<value_type> &operator++() {
        ++ptr;
        --length;
        if (!length) {
            ptr = nullptr;
        }
        return *this;
    }

    /**
     * アロー演算子を提供する
     */
    inline value_type *operator->() const {
        assert(ptr);
        return ptr;
    }

    /**
     * 参照を提供する
     */
    inline value_type &operator*() const {
        assert(ptr);
        return *ptr;
    }

    /**
     * 有効な場合はtrueを返す
     */
    operator bool() const {
        return ptr != nullptr;
    }

    /**
     * 無効な場合はtrueを返す
     */
    bool operator!() const {
        return ptr == nullptr;
    }

    /**
     * 強制的に型変換して返却する
     */
    template<typename result_type>
    result_type &as() const {
        assert(length >= sizeof(result_type)); // 変換に必要な最低容量を満たしていなければならない
        return *((result_type *) ptr);
    }
};

/**
 * 配列のポインタと長さを安全に保持する
 * 単純な配列のみをサポートし、軽量に動作することを第一とする
 */
template<typename value_type>
struct safe_array {
public:
    /**
     * 配列本体
     */
    value_type *ptr;

    int length;

    /**
     * 指定した長さの配列を確保し、古い配列を確保する
     */
    inline void refresh(const int newLength) {
        assert(newLength >= 0);

        util::safe_delete_array(&ptr);
        assert(ptr == nullptr);

        if (newLength > 0) {
            ptr = new value_type[newLength];
        } else {
            ptr = nullptr;
        }
        // 長さを保存する
        length = newLength;
    }

    /**
     * 指定した長さの配列を確保し、古い配列をmemcpyする
     */
    inline void alloc(const int newLength) {
        assert(newLength >= 0);

        value_type *pOldValues = ptr;
        if (newLength > 0) {
            ptr = new value_type[newLength];
        } else {
            ptr = nullptr;
        }

        // 古い配列が存在するならコピーする
        if (pOldValues) {
            memcpy(ptr, pOldValues, sizeof(value_type) * std::min(length, newLength));

            // 古い配列を削除する
//            SAFE_DELETE_ARRAY(pOldValues);
            util::safe_delete_array(&pOldValues);
            assert(pOldValues == nullptr);
        }

        // 長さを保存する
        length = newLength;
    }

    /**
     * 指定した長さの配列を確保し、要素をコピーする
     * operator=の起動が必要なシーンで利用する
     */
    inline void reserve(const int newLength) {
        assert(newLength >= 0);

        value_type *pOldValues = ptr;
        if (newLength > 0) {
            ptr = new value_type[newLength];
        } else {
            ptr = nullptr;
        }

        // 古い配列が存在するならコピーする
        if (pOldValues) {
            const int min_length = std::min(length, newLength);

            for (int i = 0; i < min_length; ++i) {
                ptr[i] = pOldValues[i];
            }

            // 古い配列を削除する
            util::safe_delete_array(&pOldValues);
            assert(pOldValues == nullptr);
        }

        // 長さを保存する
        length = newLength;
    }

    /**
     * メモリ領域を高速コピーする
     * operatorが不要な場合に利用する
     */
    inline void memcopyFrom(const value_type *origin, const int num) {
        // 正常な長さを持たなければならない
        assert(length >= num);
        memcpy(ptr, origin, sizeof(value_type) * num);
    }

    /**
     * メモリ領域を一つ一つコピーする
     * operatorが必要な場合に利用する
     * memcpyFromに比べて低速になる
     */
    inline void copyFrom(const value_type *origin, const int num) {
        // 正常な長さを持たなければならない
        assert(length >= num);

        for (int i = 0; i < num; ++i) {
            ptr[i] = origin[i];
        }
    }

    /**
     * 内部で持っているメモリをゼロ埋めする
     */
    inline void zeromemory() {
        ::memset(ptr, 0, sizeof(value_type) * length);
    }

    safe_array() {
        ptr = nullptr;
        length = 0;
    }

    safe_array(const int length) {
        this->ptr = nullptr;
        this->length = 0;
        alloc(length);
    }

    ~safe_array() {
        util::safe_delete_array(&ptr);
        assert(ptr == nullptr);
    }

    /**
     * 確保済みの領域を解放する
     */
    inline void clear() {
        util::safe_delete_array(&ptr);
        assert(ptr == nullptr);
        length = 0;
    }

    /**
     * オペレータアクセスを提供する
     */
    inline value_type &operator[](const int index) {
        assert(index < length);
        return ptr[index];
    }

    /**
     * オペレータアクセスを提供する
     */
    inline const value_type &operator[](const int index) const {
        assert(index < length);
        return ptr[index];
    }

    /**
     * 配列の一部を切り取ったunmanaged配列を生成する
     */
    inline unsafe_array<value_type> slice(const int index, const int length) const {
        assert(index >= 0);
        assert((index + length) <= this->length);

        unsafe_array<value_type> result;
        result.ptr = (ptr + index);
        result.length = length;
        return result;
    }

    /**
     * 配列の一部を切り取ったunmanaged配列を生成する
     */
    inline unsafe_array<value_type> slice(const int index) const {
        return slice(index, length - index);
    }

    /**
     * イテレータアクセス用のunmanaged配列を生成する
     */
    inline unsafe_array<value_type> iterator() const {
        return slice(0, length);
    }

    /**
     * 有効な場合はtrueを返す
     */
    operator bool() const {
        return ptr != nullptr;
    }

    /**
     * 無効な場合はtrueを返す
     */
    bool operator!() const {
        return ptr == nullptr;
    }

private:
    /**
     * コピーコンストラクタはサポートしない
     */
    safe_array(const safe_array &cpy);

    /**
     * コピーをサポートしない
     */
    safe_array &operator=(const safe_array &cpy);

};


namespace util {

template<typename T>
inline T *asPointer(const std::vector<T> &v, const size_t offset = 0) {
    assert(offset < v.size());
    return (T *) &(v[offset]);
}

template<typename T>
inline unsafe_array<T> unsafe(const std::vector<T> &v, const size_t offset = 0) {
    return unsafe_array<T>(asPointer(v, offset), v.size() - offset);
}

template<typename T>
inline unsafe_array<T> unsafe(const std::vector<T> &v, const size_t offset, const size_t length) {
    assert(v.size() >= offset);
    assert(v.size() >= (offset + length));
    assert(length > 0);
    return unsafe_array<T>(asPointer(v, offset), length);
}

/**
 * Vectorのメモリを指定量確保し、配列アクセスが可能なようにする。
 */
template<typename T>
inline void valloc(std::vector<T> *origin, const size_t newSize, const bool withZero) {
    origin->resize(newSize);
    if (withZero) {
        memset(&((*origin)[0]), 0, sizeof(T) * newSize);
    }

    assert(origin->capacity() == newSize);
    assert(origin->size() == newSize);
}

template<typename T>
inline void zeromemory(std::vector<T> *origin) {
    assert(origin);
    memset(&((*origin)[0]), 0, sizeof(T) * origin->size());
}

}

}

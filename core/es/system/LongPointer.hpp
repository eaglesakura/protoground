#pragma once

#include    "es/protoground.hpp"
#include    <memory>
#include    <es/memory/SafeArray.hpp>

namespace es {

/**
 * 整数変換したポインタ
 */
typedef uint64_t long_shared_pointer;

/**
 * 整数変換したポインタを扱う。
 * 主にNDKのブリッジで使用する。
 */
class LongSharedPtr {
public:
    template<typename T>
    static long_shared_pointer create(const ::std::shared_ptr<T> p) {
        ::std::shared_ptr<T> *ptr = new ::std::shared_ptr<T>(p);
        return reinterpret_cast<long_shared_pointer>(ptr);
    }

    template<typename T>
    static ::std::shared_ptr<T> get(const long_shared_pointer lp) {
        ::std::shared_ptr<T> *ptr = reinterpret_cast<::std::shared_ptr<T>*>(lp);
        if (ptr) {
            return *ptr;
        } else {
            return ::std::shared_ptr < T >(nullptr);
        }
    }

    /**
     * 新たな参照を生成する
     */
    template<typename T>
    static long_shared_pointer copy(const long_shared_pointer lp) {
        ::std::shared_ptr<T> p = get<T>(lp);
        return create<T>(p);
    }

    template<typename T>
    static void dispose(const long_shared_pointer lp) {
        if (!lp) {
            return;
        }

        ::std::shared_ptr<T> *ptr = reinterpret_cast<::std::shared_ptr<T>*>(lp);
        util::safe_delete(&ptr);
        assert(ptr == nullptr);
    }
};

}

#pragma once

#include "es/protoground.hpp"
#include "es/system/string/HashString.h"

namespace es {
namespace internal {

/**
 * 共有用文字列
 */
struct SharedString {
    string value;
    Hash128 hash128;
    Hash64 hash64;

#if defined(DEBUG)
    struct {
        /**
         * 解放した後も元の値を確認できるようにしておく
         */
        string value;
    } debug;
#endif

    SharedString(const string &value) : value(value) {
        hash128 = Hash128::from(value);
        hash64 = Hash64::from(value);

#if defined(DEBUG)
        debug.value = value;
#endif
    }

    SharedString(const string &value, const Hash128 &hash128, const Hash64 &hash64) : value(value), hash128(hash128), hash64(hash64) {
    }
};

}
}
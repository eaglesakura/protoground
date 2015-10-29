#pragma once

#include "es/protoground.hpp"
#include <map>
#include <es/memory/query_ptr.hpp>

namespace es {
namespace util {

/**
 * Mapから簡易的に値を探す
 */
template<typename keyType, typename valueType>
inline ::std::shared_ptr<valueType> find(const std::map<keyType, ::std::shared_ptr<valueType> > &objMap, const keyType &key) {
    auto itr = objMap.find(key);
    if (itr != objMap.end()) {
        // found
        return itr->second;
    } else {
        // notfound
        return ::std::shared_ptr<valueType>();
    }
}

/**
 * Mapから簡易的に値を探す
 */
template<typename keyType, typename valueType, uint64_t classId>
inline ::es::query_ptr<valueType, classId> find(const std::map<keyType, ::es::query_ptr<valueType, classId> > &objMap, const keyType &key) {
    auto itr = objMap.find(key);
    if (itr != objMap.end()) {
        // found
        return itr->second;
    } else {
        // notfound
        return ::es::query_ptr<valueType, classId>();
    }
}

}
}
#include "HashStringTable.h"
#include "es/system/string/internal/SharedString.hpp"
#include <map>
#include <set>
#include <boost/unordered_map.hpp>
#include <es/internal/log/Log.h>

namespace es {

class HashStringTable::Impl {
public:
    unsigned num = 0;

    /**
     * 128bitマッピング
     * 64bit衝突した場合に登録される
     */
    std::map<Hash128, sp<internal::SharedString> > mapping128;

    /**
     * 64bitマッピング
     * 先行して一致を確認する
     */
    boost::unordered_map<Hash64, sp<internal::SharedString> > mapping64;

    sp<internal::SharedString> find(const Hash64 &hash) const {
        auto itr = mapping64.find(hash);
        if (itr != mapping64.end()) {
            return itr->second;
        } else {
            return sp<internal::SharedString>();
        }
    }

    sp<internal::SharedString> get(const string &value) {
        Hash64 hash64 = Hash64::from(value);
        Hash128 hash128 = Hash128::from(value);

        // boost 64bitで高速チェック
        {
            auto itr = mapping64.find(hash64);
            if (itr != mapping64.end()) {
                // 高速ハッシュと低速ハッシュの両方が一致したらそれを返す
                if (itr->second->hash128 == hash128) {
                    return itr->second;
                }
            } else {
                // 高速ハッシュが登録されていないので、新規登録して返す
                sp<internal::SharedString> ref(new internal::SharedString(value, hash128, hash64));

                eslog("insert Hash64(0x%llX) Str(%s)", hash64.value(), value.c_str());

                mapping64.insert(std::make_pair(hash64, ref));
                ++num;
                return ref;
            }
        }

        // 低速ハッシュを検索する
        {
            auto itr = mapping128.find(hash128);
            if (itr != mapping128.end()) {
                // 低速ハッシュが衝突して無ければ問題ない。
                return itr->second;
            } else {
                // 低速ハッシュが登録されていないので、新規登録して返す
                sp<internal::SharedString> ref(new internal::SharedString(value, hash128, hash64));
                mapping128.insert(std::make_pair(hash128, ref));
                eslog("insert Hash128(0x%llX%llX) Str(%s)", hash128.value0(), hash128.value1(), value.c_str());
                ++num;
                return ref;
            }
        }
    }
};

HashStringTable::HashStringTable() {
    impl.reset(new HashStringTable::Impl());
}

HashStringTable::~HashStringTable() {

}

void HashStringTable::getAllStrings(std::vector<string> *result) const {
    std::set<string> values;
    for (const auto &itr : impl->mapping64) {
        values.insert(itr.second->value);
    }

    for (const auto &itr : impl->mapping128) {
        values.insert(itr.second->value);
    }

    for (const auto &str : values) {
        result->push_back(str);
    }
}

uint32_t HashStringTable::getCount() const {
    return impl->num;
}

HashString HashStringTable::get(const string &value) {
    return HashString(impl->get(value));
}

HashString HashStringTable::find(const Hash64 &hash) const {
    return HashString(impl->find(hash));
}

HashStringTable &HashStringTable::global() {
    static HashStringTable gInstance;
    return gInstance;
}

}
#pragma once

#include "es/system/string/HashString.h"
#include <vector>

namespace es {


class HashStringTable {
public:
    HashStringTable();

    ~HashStringTable();

    /**
     * 管理数を取得する
     */
    uint32_t getCount() const;

    HashString get(const string &value);

    /**
     * 検索を行う
     * 検索に失敗した場合は無効なハッシュを返す。
     */
    HashString find(const Hash64 &hash) const;

    /**
     * 管理されている全ての文字列を取得する
     */
    void getAllStrings(std::vector<string> *result) const;

    /**
     * アプリ内で共有される文字列テーブルを取得する
     */
    static HashStringTable &global();

private:
    class Impl;

    sp<Impl> impl;
};


}


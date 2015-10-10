#pragma once

#include "es/protoground.hpp"
#include "es/system/Hash64.h"
#include "es/system/Hash128.h"

namespace es {

class HashStringTable;
namespace internal {
struct SharedString;
}

/**
 * 計算済みハッシュやロード済み文字列の共有を目的とした管理クラス
 * ハッシュは一度計算すれば十分のため、キャッシュして利用できる。
 *
 * 登録された文字列は全てread-onlyとなる。
 */
class HashString {
public:
    HashString(const string &value);

    HashString();

    bool operator==(const HashString &check) const;

    bool operator!=(const HashString &check) const;

    /**
     * 有効であればtrue
     */
    bool enable() const;

    /**
     * 管理用のハッシュを取得する
     */
    const Hash128 &hash() const;

    /**
     * 高速管理用のハッシュを生成する
     * 多少衝突体制が低くても問題ない場合はこちらを利用したほうがboostの高速mapが利用できるので良い。
     */
    const Hash64 &shortHash() const;

    /**
     * 本来の文字列を取得する
     */
    const string &get() const;

    /**
     * ハッシュだけがあれば問題ない場合に、文字列を解放する。
     */
    HashString &clearValue();

private:
    HashString(const sp<internal::SharedString> &ref) : ref(ref) { }

    friend class HashStringTable;

    sp<internal::SharedString> ref;
};

}




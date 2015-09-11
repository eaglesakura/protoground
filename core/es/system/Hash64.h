#pragma once

#include "es/protoground.hpp"
#include <thread>

namespace es {

/**
 * 64bitのハッシュを扱う
 *
 * 32bitの場合は型情報をハッシュに含めるので、ビットの並びが同じでも違うハッシュ値となる。
 * ビットの並びで生成されるので、64bitの場合は型が違っても同じビットの並びの場合、同じハッシュ値が生成される。
 * プリミティブ型の場合、元の型よりも大きくなる場合があるので、利用箇所には注意が必要
 * string等の長いbit数の可変長データを扱う場合、衝突危険性が増すことに注意。
 *
 * プリミティブ型: ビット情報のみをハッシュの対象とする
 * ポインタ型: メモリアドレスのみをハッシュの対象とする
 * string: boostの64bitをハッシュの対象とする
 *
 * プリミティブとして扱えるbit数のため、ハッシュ値の取得を行える。
 * boostのhash_value互換として利用できる。
 */
struct Hash64 {
    Hash64();

    bool operator<(const Hash64 &check) const;

    bool operator==(const Hash64 &check) const;

    bool operator!=(const Hash64 &check) const;

    /**
     * 64bitハッシュ値を取得する
     */
    uint64_t value() const;

    /**
     * デバッグ用文字列に変換する
     */
    string toString() const;

    static Hash64 from(const int8_t value);

    static Hash64 from(const uint8_t value);

    static Hash64 from(const int16_t value);

    static Hash64 from(const uint16_t value);

    static Hash64 from(const int32_t value);

    static Hash64 from(const uint32_t value);

    static Hash64 from(const int64_t value);

    static Hash64 from(const uint64_t value);

    static Hash64 from(const float value);

    static Hash64 from(const double value);

    static Hash64 from(const string &value);

    /**
     * Thread IDを変換する
     */
    static Hash64 from(const std::thread::id &id);

    static Hash64 fromCurrentThread();

    /**
     * メモリアドレスからハッシュを生成する
     */
    static Hash64 from(const void *value);

    /**
     * ハッシュを復元する
     */
    static Hash64 restore(const uint64_t value);

private:
    uint64_t hash64 = 0;
};

static_assert(sizeof(Hash64) == (64 / 8), "Hash64 != 64bit");

/**
 * 通常利用する場合はビット数を意識させない
 */
typedef Hash64 hash;

/**
 * boostへ提供するハッシュ関数
 * 通常、使用することはない。
 */
size_t hash_value(const Hash64 &hash);

}

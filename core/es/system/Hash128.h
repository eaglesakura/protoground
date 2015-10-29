#pragma once

#include "es/protoground.hpp"

namespace es {

/**
 * 128bitのハッシュを扱う
 *
 * 型とビットの並びそれぞれにハッシュが生成されるので、元のビットが同じでも型によって違うハッシュが生成される
 * プリミティブ型の場合、元の型よりも大きくなってしまうので、利用箇所には注意が必要
 * それなりの衝突体制を持つ
 *
 * プリミティブ型: 型情報とビット情報を含めてハッシュの対象とする
 * ポインタ型: メモリアドレスのみをハッシュの対象とする
 * string: 簡易に生成した64bitと32bitのハッシュ及び文字列長をチェックする
 *
 * ハッシュ化された計算値は64bit, 32bit, 16bitにそれぞれ分解されていて、残16bitは今のところ未使用のまま予約(0x0000固定)されている。
 * 32bit CPUの場合は32bit -> 64bit -> 16bitの順に比較される。
 * 64bit CPUの場合は64bit -> 32bit -> 16bitの順に比較される。
 */
struct Hash128 {
    Hash128();

    uint64_t value0() const;

    uint64_t value1() const;

    bool operator<(const Hash128 &check) const;

    bool operator==(const Hash128 &check) const;

    bool operator!=(const Hash128 &check) const;

    /**
     * デバッグ用文字列に変換する
     */
    string toString() const;

    static Hash128 from(const int8_t value);

    static Hash128 from(const uint8_t value);

    static Hash128 from(const int16_t value);

    static Hash128 from(const uint16_t value);

    static Hash128 from(const int32_t value);

    static Hash128 from(const uint32_t value);

    static Hash128 from(const int64_t value);

    static Hash128 from(const uint64_t value);

    static Hash128 from(const float value);

    static Hash128 from(const double value);

    static Hash128 from(const string &value);

    /**
     * メモリアドレスからハッシュを生成する
     *
     * ポインタは仕組み上ビット情報の重複はあり得ないため、純粋にアドレスのみが対象となる。
     */
    static Hash128 from(const void *value);

private:
    uint64_t hash64 = 0;

    uint32_t hash32 = 0;

    /**
     * 2番目に判定されるメタ情報
     * 型情報も格納されるため、型が違えば同じ数値でも全く違うハッシュとして判定される。
     *
     * string: 文字列長も判定材料とする。同じ文字列長かつ同じ2種類のハッシュ値を持つ文字列は事実上無視できるほど少ない。
     */
    union {
        uint32_t bits;

        struct {
            /**
             * 型情報
             */
            uint8_t type;

            /**
             * 型ごとの拡張機能
             * stringの場合、文字列長の下1byteを入力しておく
             */
            uint8_t type_ex0;
            uint8_t reserved0;
            uint8_t reserved1;
        } meta;
    } ex;
};

static_assert(sizeof(Hash128) == (128 / 8), "Hash128 != 128bit");

}

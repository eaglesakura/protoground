#include "hash64.h"
#include "Hash128.h"

#include "es/util/StringUtil.h"
#include <thread>
#include <functional>

namespace es {

namespace {

enum ValueType_e {
    ValueType_int8,
    ValueType_uint8,
    ValueType_int16,
    ValueType_uint16,
    ValueType_int32,
    ValueType_uint32,
    ValueType_int64,
    ValueType_uint64,
    ValueType_float,
    ValueType_double,
    ValueType_string,
    ValueType_pointer,
    ValueType_thread_id,
};

}


Hash64::Hash64() : hash64(0) {
}

uint64_t Hash64::value() const {
    return hash64;
}

bool Hash64::operator<(const Hash64 &check) const {
    return hash64 < check.hash64;
}

bool Hash64::operator==(const Hash64 &check) const {
    // 衝突する可能性の高い情報からチェックを行う
    return hash64 == check.hash64;
}

bool Hash64::operator!=(const Hash64 &check) const {
    // 衝突する可能性の高い情報からチェックを行う
    return hash64 != check.hash64;
}

string Hash64::toString() const {
    // 64bit
    const uint32_t hash64_0 = (uint32_t) (hash64 >> 32 & 0xFFFFFFFF);
    const uint32_t hash64_1 = (uint32_t) (hash64 & 0xFFFFFFFF);
    return util::format(32, "%04x-%04x", hash64_0, hash64_1);
}

Hash64 Hash64::from(const int8_t value) {
    Hash64 result;
    result.hash64 = static_cast<uint64_t >(value) & 0xFF;
    result.hash64 |= ((uint64_t) ValueType_int8 << 32);
    return result;
}

Hash64 Hash64::from(const uint8_t value) {
    Hash64 result;
    result.hash64 = static_cast<uint64_t  >(value) & 0xFF;
    result.hash64 |= ((uint64_t) ValueType_uint8 << 32);
    return result;
}

Hash64 Hash64::from(const int16_t value) {
    Hash64 result;
    result.hash64 = static_cast<uint64_t  >(value) & 0xFFFF;
    result.hash64 |= ((uint64_t) ValueType_int16 << 32);
    return result;
}

Hash64 Hash64::from(const uint16_t value) {
    Hash64 result;
    result.hash64 = static_cast<uint64_t  >(value) & 0xFFFF;
    result.hash64 |= ((uint64_t) ValueType_uint16 << 32);
    return result;
}

Hash64 Hash64::from(const int32_t value) {
    Hash64 result;
    result.hash64 = static_cast<uint64_t  >(value) & 0xFFFFFFFF;
    result.hash64 |= ((uint64_t) ValueType_int32 << 32);
    return result;
}

Hash64 Hash64::from(const uint32_t value) {
    Hash64 result;
    result.hash64 = static_cast<uint64_t  >(value) & 0xFFFFFFFF;
    result.hash64 |= ((uint64_t) ValueType_uint32 << 32);
    return result;
}

Hash64 Hash64::from(const int64_t value) {
    Hash64 result;
    result.hash64 = static_cast<uint64_t >(value);
    return result;
}

Hash64 Hash64::from(const uint64_t value) {
    Hash64 result;
    result.hash64 = value;
    return result;
}

Hash64 Hash64::from(const float value) {
    static_assert(sizeof(float) == sizeof(uint32_t), "bit length error!!");

    Hash64 result;
    result.hash64 = *((uint32_t *) &value);
    result.hash64 |= ((uint64_t) ValueType_float << 32);
    return result;
}

Hash64 Hash64::from(const double value) {
    static_assert(sizeof(double) == sizeof(uint64_t), "bit length error!!");

    Hash64 result;
    result.hash64 = *((uint64_t *) &value);
    return result;
}

namespace {
	
static uint64_t make_hash(const uint8_t *buffer, int length, uint64_t seed) {
    assert(buffer);
	assert(length);

    uint64_t result = seed;
    while (length--) {
        assert(seed);
        result ^= ((*buffer) & 0xFF) * seed;
        seed = ~((seed << 7) | ((seed >> 57) & 0x00FFFFFFFFFFFFFFL));
        ++buffer;
    }

    assert(result);
    return result;
}

static uint64_t make_hash(const uint8_t *str, uint64_t seed) {
    assert(str);

    uint64_t result = seed;
    while (*str) {
        assert(seed);
        result ^= ((*str) & 0xFF) * seed;
        seed = ~((seed << 7) | ((seed >> 57) & 0x00FFFFFFFFFFFFFFL));
        ++str;
    }

    assert(result);
    return result;
}

}

Hash64 Hash64::from(const string &value) {
    Hash64 result;
#if 0 /* boost版はコンパイル環境でビット長が異なるため、使用できない */
    size_t hash = boost::hash<std::string>()(value);
    result.hash64 = static_cast<uint64_t >(hash);
#else
    result.hash64 = make_hash((uint8_t *) value.c_str(), 0x123456789ABCD012L);
#endif
    return result;
}

Hash64 Hash64::from(const std::thread::id &id) {
    Hash64 result;
    result.hash64 = make_hash((uint8_t*)&id, sizeof(id), 0x1234567887654321L);
    return result;
}

Hash64 Hash64::fromCurrentThread() {
    return from(std::this_thread::get_id());
}

Hash64 Hash64::from(const void *value) {
    Hash64 result;
    result.hash64 = (uint64_t) (value);
#if defined(BUILD_32bit)
    // 32bitビルドは上位32bitのみを取り出して型情報を含める
    result.hash64 = result.hash64 & 0xFFFFFFFF;
    result.hash64 |= ((uint64_t) ValueType_pointer << 32);
#endif
    return result;
}

Hash64 Hash64::restore(const uint64_t value) {
    Hash64 result;
    result.hash64 = value;
    return result;
}

Hash128::Hash128() : hash64(0), hash32(0) {
    ex.bits = 0;
}

bool Hash128::operator<(const Hash128 &check) const {
#if defined(BUILD_64bit)
    if(hash64 == check.hash64) {
        return hash32 < check.hash32;
    } else {
        return hash64 < check.hash64;
    }
#else
    if (hash32 == check.hash32) {
        return hash64 < check.hash64;
    } else {
        return hash32 < check.hash32;
    }
#endif
}

bool Hash128::operator==(const Hash128 &check) const {
    // 衝突する可能性の高い情報からチェックを行う
#if defined(BUILD_64bit)
    return hash64 == check.hash64 &&
           ex.bits == check.ex.bits &&
           hash32 == check.hash32;
#else
    return hash32 == check.hash32 &&
           hash64 == check.hash64 &&
           ex.bits == check.ex.bits;
#endif
}

bool Hash128::operator!=(const Hash128 &check) const {
    // 衝突する可能性の高い情報からチェックを行う
#if defined(BUILD_64bit)
    return hash64 != check.hash64 ||
           ex.bits != check.ex.bits ||
           hash32 != check.hash32;
#else
    return hash32 != check.hash32 ||
           hash64 != check.hash64 ||
           ex.bits != check.ex.bits;
#endif
}

string Hash128::toString() const {
    // 64bit
    const uint32_t hash64_0 = (uint32_t) (hash64 >> 32 & 0xFFFFFFFF);
    const uint32_t hash64_1 = (uint32_t) (hash64 & 0xFFFFFFFF);
    return util::format(32, "%04x-%04x-%04x-%x", hash64_0, hash64_1, hash32, ex.bits);
}

Hash128 Hash128::from(const int8_t value) {
    Hash128 result;
    result.hash64 = static_cast<uint64_t >(value) & 0xFF;
    result.hash64 |= ((uint64_t) ValueType_int8 << 32);
    result.hash32 = (uint32_t) result.hash64;
    result.ex.meta.type = ValueType_int8;
    return result;
}

Hash128 Hash128::from(const uint8_t value) {
    Hash128 result;
    result.hash64 = static_cast<uint64_t  >(value) & 0xFF;
    result.hash64 |= ((uint64_t) ValueType_uint8 << 32);
    result.hash32 = (uint32_t) result.hash64;
    result.ex.meta.type = ValueType_uint8;
    return result;
}

Hash128 Hash128::from(const int16_t value) {
    Hash128 result;
    result.hash64 = static_cast<uint64_t  >(value) & 0xFFFF;
    result.hash64 |= ((uint64_t) ValueType_int16 << 32);
    result.hash32 = (uint32_t) result.hash64;
    result.ex.meta.type = ValueType_int16;
    return result;
}

Hash128 Hash128::from(const uint16_t value) {
    Hash128 result;
    result.hash64 = static_cast<uint64_t  >(value) & 0xFFFF;
    result.hash64 |= ((uint64_t) ValueType_uint16 << 32);
    result.hash32 = (uint32_t) result.hash64;
    result.ex.meta.type = ValueType_uint16;
    return result;
}

Hash128 Hash128::from(const int32_t value) {
    Hash128 result;
    result.hash64 = static_cast<uint64_t  >(value) & 0xFFFFFFFF;
    result.hash64 |= ((uint64_t) ValueType_int32 << 32);
    result.hash32 = (uint32_t) result.hash64;
    result.ex.meta.type = ValueType_int32;
    return result;
}

Hash128 Hash128::from(const uint32_t value) {
    Hash128 result;
    result.hash64 = static_cast<uint64_t  >(value) & 0xFFFFFFFF;
    result.hash64 |= ((uint64_t) ValueType_uint32 << 32);
    result.hash32 = (uint32_t) result.hash64;
    result.ex.meta.type = ValueType_uint32;
    return result;
}

Hash128 Hash128::from(const int64_t value) {
    Hash128 result;
    result.hash64 = static_cast<uint64_t >(value);
    result.hash32 = (uint32_t) result.hash64;
    result.ex.meta.type = ValueType_int64;
    return result;
}

Hash128 Hash128::from(const uint64_t value) {
    Hash128 result;
    result.hash64 = value;
    result.hash32 = ((uint32_t) result.hash64) ^ ((uint32_t) (result.hash64 >> 32));
    result.ex.meta.type = ValueType_uint64;
    return result;
}

Hash128 Hash128::from(const float value) {
    static_assert(sizeof(float) == sizeof(uint32_t), "bit length error!!");

    Hash128 result;
    result.hash64 = (*((uint32_t *) &value)) & 0xFFFFFFFF;
    result.hash64 |= ((uint64_t) ValueType_float << 32);
    result.hash32 = (uint32_t) result.hash64;
    result.ex.meta.type = ValueType_float;
    return result;
}

Hash128 Hash128::from(const double value) {
    static_assert(sizeof(double) == sizeof(uint64_t), "bit length error!!");

    Hash128 result;
    result.hash64 = *((uint64_t *) &value);
    result.hash32 = ((uint32_t) result.hash64) ^ ((uint32_t) (result.hash64 >> 32));
    result.ex.meta.type = ValueType_double;
    return result;
}

Hash128 Hash128::from(const string &value) {
    Hash128 result;
#if 0 /* ビット長を固定するため、適当なハッシュ関数を使用する */
    size_t hash = boost::hash<std::string>()(value);
    result.hash64 = static_cast<uint64_t >(hash);
#else
    result.hash64 = make_hash((uint8_t *) value.c_str(), 0x9ABCD12312345678L);
#endif

    // 適当な32bitハッシュも生成する
    // boost::hashと適当ハッシュの64bit+32bit+文字列長8bitが競合することはほとんど無いと考えられる
    result.hash32 = (uint32_t) make_hash((uint8_t *) value.c_str(), 0xA1A2B3B4CCDDEEFFL);

    result.ex.meta.type = ValueType_string;
    result.ex.meta.type_ex0 = (uint8_t) value.length();

    return result;
}

Hash128 Hash128::from(const void *value) {
    Hash128 result;
    result.hash64 = (uint64_t) (value);
#if defined(BUILD_32bit)
    // 32bitビルドは上位32bitのみを取り出す
    result.hash64 = result.hash64 & 0xFFFFFFFF;
    result.hash64 |= ((uint64_t) ValueType_pointer << 32);
#endif
    result.hash32 = (uint32_t) result.hash64;
    result.ex.meta.type = ValueType_pointer;
    return result;
}

uint64_t Hash128::value0() const {
    return hash64;
}

uint64_t Hash128::value1() const {
    return ((uint64_t) hash32) << 32 | (((uint64_t) ex.bits) & 0xFFFFFFFFL);
}

/**
 * boostへ提供するハッシュ関数
 * 通常、使用することはない。
 */
size_t hash_value(const Hash64 &hash) {
    return (size_t) hash.value();
}


}
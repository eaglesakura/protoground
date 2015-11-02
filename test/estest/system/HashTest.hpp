#pragma once

#include "estest/protoground-test.hpp"
#include "es/system/Hash64.h"
#include "es/system/Hash128.h"
#include "es/system/string/HashString.h"
#include "es/system/string/HashStringTable.h"

namespace es {
namespace test {

TEST(HashTest, Check64) {
    {
        Hash64 h0 = Hash64::from(string("test value0"));
        Hash64 h1 = Hash64::from(string("test value1"));

        ASSERT_FALSE(h0 == h1);
    }
    {
        Hash128 h0 = Hash128::from(string("test value0"));
        Hash128 h1 = Hash128::from(string("test value1"));
        Hash128 h2 = Hash128::from(string("test value2"));

        ASSERT_FALSE(h0 == h1);
        ASSERT_FALSE(h1 == h2);
        ASSERT_FALSE(h0 == h2);
    }
    {
        Hash128 h0 = Hash128::from(uint8_t(128));
        Hash128 h1 = Hash128::from(int16_t(128));
        Hash128 h2 = Hash128::from(int32_t(128));

        ASSERT_FALSE(h0 == h1);
        ASSERT_FALSE(h1 == h2);
        ASSERT_FALSE(h0 == h2);
    }
    {
        Hash128 h0 = Hash128::from(float(12.3f));
        Hash128 h1 = Hash128::from(double(12.3));

        ASSERT_FALSE(h0 == h1);
    }
}

TEST(HashStringTest, HashString) {
    const auto oldGlobalStrings = HashStringTable::global().getCount();

    auto t0 = HashStringTable::global().get("text0");
    auto t1 = HashStringTable::global().get("text1");
    ASSERT_NE(t0, t1);
    ASSERT_EQ(t0.get(), string("text0"));
    ASSERT_EQ(t1.get(), string("text1"));

    auto t0a = HashStringTable::global().get("text0");
    ASSERT_EQ(t0, t0a);
    ASSERT_NE(t1, t0a);
    ASSERT_EQ(t0a.get(), string("text0"));

    ASSERT_EQ(HashStringTable::global().getCount() - oldGlobalStrings, 2);

    // メモリを解放し、共有している側も解放されることを確認
    t0a.clearValue();
    ASSERT_EQ(t0a.get(), string(""));
    ASSERT_EQ(t0.get(), string(""));
}

}
}
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
        hash h0 = hash::from(string("test value0"));
        hash h1 = hash::from(string("test value1"));

        ASSERT_FALSE(h0 == h1);
    }
    {
        long_hash h0 = long_hash::from(string("test value0"));
        long_hash h1 = long_hash::from(string("test value1"));
        long_hash h2 = long_hash::from(string("test value2"));

        ASSERT_FALSE(h0 == h1);
        ASSERT_FALSE(h1 == h2);
        ASSERT_FALSE(h0 == h2);
    }
    {
        long_hash h0 = long_hash::from(uint8_t(128));
        long_hash h1 = long_hash::from(int16_t(128));
        long_hash h2 = long_hash::from(int32_t(128));

        ASSERT_FALSE(h0 == h1);
        ASSERT_FALSE(h1 == h2);
        ASSERT_FALSE(h0 == h2);
    }
    {
        long_hash h0 = long_hash::from(float(12.3f));
        long_hash h1 = long_hash::from(double(12.3));

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
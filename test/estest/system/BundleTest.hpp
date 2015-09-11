#pragma once

#include "estest/protoground-test.hpp"
#include "es/system/Bundle.h"

namespace es {
namespace test {

/**
 * Bundleの入出力が正常に行えることを検証する
 */
TEST(BundleTest, BundleAddGet) {
    Bundle bundle;
    ASSERT_FALSE(bundle.hasValue("key0"));

    ASSERT_TRUE(bundle.getInt32("errorKey", -1) == -1);

    bundle.putInt32("intKey", 32);
    bundle.putInt64("longKey", 64);
    bundle.putString("stringKey", "stringValue");
    bundle.putFloat("floatKey", 1.23f);

    ASSERT_EQ(bundle.getInt32("intKey", 0), 32);
    ASSERT_EQ(bundle.getInt64("longKey", 0), 64);
    ASSERT_EQ(bundle.getString("stringKey", "error"), "stringValue");
    ASSERT_EQ(bundle.getFloat("floatKey", 0), 1.23f);

    // 同じKeyは、先勝ちになる
    bundle.putInt32("intKey", 128);
    ASSERT_TRUE(bundle.getInt32("intKey", 0) != 128);

    // 値を排除して追加し直す
    bundle.clear("intKey");
    ASSERT_FALSE(bundle.hasValue("intKey"));
    bundle.putInt32("intKey", 128);
    ASSERT_EQ(bundle.getInt32("intKey", 0), 128);


    Bundle bundle2;
    bundle2.putBundle("bundleKey", bundle);
    ASSERT_TRUE(bundle2.hasValue("bundleKey"));
    ASSERT_TRUE(bundle2.getBundle("errorKey").isEmpty());
    ASSERT_EQ(bundle2.getBundle("bundleKey").getFloat("floatKey", 0), 1.23f);

}

}
}
#pragma once

#include "es/protoground.hpp"
#include "es/system/string/HashString.h"

namespace es {

/**
 * 複数の型のKey-Valueを一括で管理する
 *
 * putされた値は先勝になるため、後から入力された値は無視される。
 *
 * operator=はデフォルト動作させており、複数Bundleで共有される場合がある。
 * その場合は同じ内容を示す。
 *
 * 内部は64bitの高速ハッシュで実装されているため、keyは可能な限り重複しないように連番文字列(arg0, arg1のように）にしないほうが良い。
 * ハッシュの実装についてはHash64を参照する。
 * 現実的にはまず重複することはない。
 */
class Bundle {
public:
    Bundle() = default;

    /**
     * 指定されたキーの値を保持していたらtrue
     */
    bool hasValue(const string &key) const;

    /**
     * キーをひとつも持っていない場合true
     */
    bool isEmpty() const;

    /**
     * 保存されている値を排除する
     */
    void clear();

    /**
     * 指定されたキーが保存されていたら削除する
     */
    void clear(const string &key);

    bool getBool(const string &key, const bool defValue) const;

    bool getBool(const HashString &key, const bool defValue) const;

    int32_t getInt32(const string &key, const int32_t defValue) const;

    int32_t getInt32(const HashString &key, const int32_t defValue) const;

    int64_t getInt64(const string &key, const int64_t defValue) const;

    int64_t getInt64(const HashString &key, const int64_t defValue) const;

    float getFloat(const string &key, const float defValue) const;

    float getFloat(const HashString &key, const float defValue) const;


    double getDouble(const string &key, const double defValue) const;

    double getDouble(const HashString &key, const double defValue) const;

    string getString(const string &key, const string &defValue) const;
    string getString(const HashString &key, const string &defValue) const;

    std::shared_ptr<Object> getObject(const HashString &key) const;

    /**
     * Bundleを取得する
     * keyが見つからない場合、空のBundleを返す。
     */
    Bundle getBundle(const string &key) const;

    Bundle &putBool(const HashString &key, const bool value);

    Bundle &putInt32(const string &key, const int32_t value);

    Bundle &putInt32(const HashString &key, const int32_t value);

    Bundle &putInt64(const string &key, const int64_t value);

    Bundle &putInt64(const HashString &key, const int64_t value);

    Bundle &putFloat(const string &key, const float value);

    Bundle &putFloat(const HashString &key, const float value);

    Bundle &putDouble(const string &key, const double value);

    Bundle &putDouble(const HashString &key, const double value);

    Bundle &putString(const string &key, const string &value);
    Bundle &putString(const HashString &key, const string &value);

    Bundle &putBundle(const string &key, const Bundle &value);

    Bundle &putObject(const HashString &key, const std::shared_ptr<Object> &value);

    ~Bundle() = default;

private:
    class Impl;

    sp<Impl> impl;

    sp<Impl> get();
};

}



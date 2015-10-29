#pragma once

#include "es/protoground.hpp"

namespace es {

class IStringConverter {
public:
    /**
     * UTF8文字列をワイド文字列に変換する
     */
    virtual wide_string toWideString(const string &utf8) = 0;

    /**
     * ワイド文字列をUTF8に戻す
     */
    virtual string toUtf8String(const wide_string &wide) = 0;

    virtual ~IStringConverter() = default;

};

}
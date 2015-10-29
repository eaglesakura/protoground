#pragma once

#include "es/system/string/IStringConverter.h"

#if defined(BUILD_MacOSX)

#include <codecvt>
#include <locale>

namespace es {
namespace internal {

class Cpp11StringConverter : public IStringConverter {
public:
    Cpp11StringConverter() = default;

    virtual ~Cpp11StringConverter() = default;

    /**
     * UTF8文字列をワイド文字列に変換する
     */
    virtual wide_string toWideString(const std::string &utf8) override {
        return conversion.from_bytes(utf8);
    }


    virtual string toUtf8String(const wide_string &wide) override {
        return conversion.to_bytes(wide);
    }

private:
    std::wstring_convert<std::codecvt_utf8_utf16<wide_char>, wide_char> conversion;
};

}
}

#endif

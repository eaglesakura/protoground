#pragma once

#include "es/system/string/IStringConverter.h"

#if defined(BUILD_MacOSX)

#include <codecvt>
#include <locale>

namespace es {
namespace internal {

class Cpp11StringConverter : public IStringConverter {
public:
    Cpp11StringConverter();

    virtual ~Cpp11StringConverter();

    /**
     * UTF8文字列をワイド文字列に変換する
     */
    virtual std::wstring toWideString(const std::string &utf8) override;


    virtual string toUtf8String(const wide_string &wide) override;

private:
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
};

}
}

#endif
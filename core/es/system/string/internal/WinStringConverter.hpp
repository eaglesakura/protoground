#pragma once


#include "es/system/string/IStringConverter.h"

#if defined(BUILD_Windows)
#include <Windows.h>
#include <vector>

namespace es {
namespace internal {

class WinStringConverter : public IStringConverter {
public:
    WinStringConverter() = default;
    virtual ~WinStringConverter() = default;

    /**
     * UTF8文字列をワイド文字列に変換する
     */
    virtual wide_string toWideString(const string &utf8) override {
        const unsigned len = ::MultiByteToWideChar( CP_UTF8, MB_PRECOMPOSED, utf8.c_str(), -1, NULL, 0 );
        
        std::vector<wide_char>  buffer( len + 1 );
        buffer[len] = 0;

        ::MultiByteToWideChar( CP_UTF8, MB_PRECOMPOSED, utf8.c_str(), -1, (LPWSTR)&buffer[0], len );

        return wide_string(&buffer[ 0 ]);
    }

    /**
     * ワイド文字列をUTF8に戻す
     */
    virtual string toUtf8String(const wide_string &wide) override {
        const unsigned len = ::WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)wide.c_str(), -1, NULL, 0, NULL, NULL);

        std::vector<char> buffer(len + 1);
        buffer[len] = 0;

        ::WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)wide.c_str(), -1, &buffer[0], 0, NULL, NULL);

        return string(&buffer[0]);
    }

private:

};

}
}
#endif
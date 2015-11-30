#pragma once

#include "es/system/string/IStringConverter.h"

#if defined(BUILD_Cygwin)

#include "es/memory/SafeArray.hpp"
#include <iconv.h>
#include <vector>

namespace es {
namespace internal {

class IconvStringConverter : public es::IStringConverter {
public:
    IconvStringConverter() {
        wideToUtf = ::iconv_open("UTF-8", "UTF-32LE");
        utfToWide = ::iconv_open("UTF-32LE", "UTF-8");

        assert(wideToUtf);
        assert(utfToWide);
    }

    virtual ~IconvStringConverter() {
        ::iconv_close(wideToUtf);
        ::iconv_close(utfToWide);
    }


    virtual wide_string toWideString(const string &utf8) {
        iconv_t ic = utfToWide;

        std::vector<uint8_t> outbuf((utf8.length() + 1) * sizeof(wide_char));
        util::zeromemory(&outbuf);

        char *in = (char *) utf8.c_str();
        char *out = (char *) util::asPointer(outbuf);
        size_t in_size = (size_t) utf8.size();
        size_t out_size = (size_t) outbuf.size();
        iconv(ic, &in, &in_size, &out, &out_size);

        return wide_string((wide_char *) util::asPointer(outbuf));
    }

    virtual string toUtf8String(const wide_string &wide) {
        iconv_t ic = wideToUtf;
        int32_t bufferSize = wide.length() * sizeof(wide_char);

        std::vector<uint8_t> inbuf(bufferSize + 1);
        std::vector<uint8_t> outbuf(bufferSize + sizeof(wide_char));

        util::zeromemory(&inbuf);
        util::zeromemory(&outbuf);

        char *in = (char *) util::asPointer(inbuf);
        char *out = (char *) util::asPointer(outbuf);
        size_t in_size = (size_t) bufferSize;
        size_t out_size = (size_t) bufferSize;
        memcpy(in, wide.c_str(), wide.length() * sizeof(wide_char));

        iconv(ic, &in, &in_size, &out, &out_size);

        return string((char *) util::asPointer(outbuf));
    }

private:
    iconv_t wideToUtf;
    iconv_t utfToWide;
};

}
}

#endif
#include "Cpp11StringConverter.h"

#if defined(BUILD_MacOSX)

#include <codecvt>

namespace es {
namespace internal {

Cpp11StringConverter::Cpp11StringConverter() {

}

Cpp11StringConverter::~Cpp11StringConverter() {

}

wide_string Cpp11StringConverter::toWideString(const std::string &utf8) {
    return conversion.from_bytes(utf8);
}

string Cpp11StringConverter::toUtf8String(const wide_string &wide) {
    return conversion.to_bytes(wide);
}
}
}

#endif
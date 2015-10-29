#include    "es/protoground.hpp"
#include    "Log.h"

#if defined(BUILD_Android)
#include    "android/log.h"

namespace {

/**
 * ログ出力を行う
 */
void logAndroid(const es::internal::LogType_e type, const char *__file, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    static const int LOG_TYPES[] = { ANDROID_LOG_INFO, ANDROID_LOG_DEBUG, ANDROID_LOG_ERROR, };
    __android_log_vprint(LOG_TYPES[type], __file, fmt, ap);

    va_end(ap);
}

}

namespace es {
internal::Logger::LogFunctionPtr internal::Logger::func = logAndroid;
}

#elif !defined(BUILD_iOS)
#include <string.h>
#include <cstdarg>
#include <stdio.h>

namespace {

/**
 * ログ出力を行う
 */
void logBasic(const es::internal::LogType_e type, const char *__file, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    printf("%s | ", __file); // ファイル名出力
    vprintf(fmt, ap); // 通常ログ出力
    printf("\n"); // コンソール改行

    va_end(ap);
}

}
namespace es {
internal::Logger::LogFunctionPtr internal::Logger::func = logBasic;

}

#endif

namespace es {

namespace internal {

const char *pathToFileName(const char *__file__) {
    char separator = '/';
#if defined(BUILD_Android) || defined(BUILD_Windows)
    // cygwin check
    if (__file__[1] == ':') {
        // Windows形式だった場合、セパレータが異なる
        separator = '\\';
    }
#endif

    return strrchr(__file__, separator) + 1;
}

}
}

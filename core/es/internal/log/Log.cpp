#include    "es/protoground.hpp"
#include    "Log.h"

#if defined(BUILD_Android)

#include    "android/log.h"

namespace {

/**
 * ログ出力を行う
 */
void logAndroid(const es::internal::LogType_e type, const char *__file, const char *fmt, ...) {
    assert(__file);
    assert(fmt);

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

#elif defined(BUILD_Windows)

#include <Windows.h>
#include <string.h>
#include <cstdarg>
#include "es/memory/SafeArray.hpp"

namespace {

/**
* ログ出力を行う
*/
void logWindows(const es::internal::LogType_e type, const char *__file, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    std::vector<char> buffer(strlen(__file) + strlen(fmt) + 512);

    {
        sprintf(es::util::asPointer(buffer), "%s | ", __file); // ファイル名出力
        OutputDebugString(es::util::asPointer(buffer));
    }
    {
        vsprintf(es::util::asPointer(buffer), fmt, ap);
        OutputDebugString(es::util::asPointer(buffer));
    }
    OutputDebugString("\n");
    va_end(ap);
}

}
namespace es {
internal::Logger::LogFunctionPtr internal::Logger::func = logWindows;

}

#else
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
    // Windows Check
        if (__file__[1] == ':' && __file__[2] == '\\') {
            // C:\ 形式の場合、Windows形式のパスとして扱う
            separator = '\\';
        }
#endif
        auto result = strrchr(__file__, separator);
        if (result) {
            return result + 1;
        } else {
            return __file__;
        }
    }

}
}

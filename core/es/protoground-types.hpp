#pragma once

#include    <limits.h>

/**
 * BUILD_XXXでプラットフォームを示す。
 *
 * BUILD_Android
 * BUILD_iOS
 * BUILD_MacOSX
 */
#if defined(__ANDROID__)
#   define BUILD_Android
#elif defined(__MACH__)
#   define BUILD_MacOSX
#elif defined(__APPLE__)
#   define BUILD_iOS
#elif defined(__CYGWIN__)
#define BUILD_Windows
#define BUILD_Windows_Cygwin
#else
#error  Unknown Platform
#endif

/**
 * アーキテクチャ32bit/64bit判定を行う
 */
#if ((ULONG_MAX) == (UINT_MAX))
static_assert(sizeof(void *) == 4, "sizeof(void*) != 32bit");
#define BUILD_32bit    1
#else
static_assert(sizeof(void *) == 8, "sizeof(void*) != 64bit");
#define BUILD_64bit    1
#endif

/**
 * 利用可能なレンダラが定義される。
 *
 * #define ES_GRAPHICS_OPENGL  1
 * #define ES_GRAPHICS_METAL   1
 * #define ES_GRAPHICS_DX11    1
 */
#if defined(BUILD_Android)
// AndroidはOpenGL ES必須
#define ES_GRAPHICS_OPENGL  1
#endif

#if defined(BUILD_MacOSX)
// Mac OS Xは今のところOpenGL 4.1限定
#define ES_GRAPHICS_OPENGL  1
#endif

#if defined(BUILD_iOS)
#define ES_GRAPHICS_OPENGL  1
#endif

#if !defined(DEBUG) && (!defined(NDEBUG) || defined(ES_BUILD_GTEST_MODE))
#define DEBUG 1
#endif

/**
 * 非推奨なメソッドをラップする
 */
#define PROTOGROUND_DEPRECATED(__METHOD__)        __METHOD__ __attribute__((deprecated))


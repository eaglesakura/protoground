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
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define BUILD_Windows
#define BUILD_64bit
#include <Windows.h>
#else
#error  Unknown Platform
#endif

/**
 * アーキテクチャ32bit/64bit判定を行う
 */
#if !defined(BUILD_32bit) && !defined(BUILD_64bit)
#if ((ULONG_MAX) == (UINT_MAX))
#define BUILD_32bit    1
#else
#define BUILD_64bit    1
#endif
#endif

#if defined(BUILD_32bit)
static_assert(sizeof(void *) == 4, "sizeof(void*) != 32bit");
#else
static_assert(sizeof(void *) == 8, "sizeof(void*) != 64bit");
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

/**
 * Alignmentを指定する
 */
#define PGD_DATA_ALIGN(type, __BYTES__)       type __attribute__((aligned(__BYTES__)))


#if defined(BUILD_Windows)

 /**
 * ファイル書き込みを行うオブジェクトのメモリサイズを指定する
 */
#define PGD_FILE_ALIGN_OBJECT_BEGIN

 /**
 * ファイル書き込みを行うオブジェクトのメモリサイズを指定する
 */
#define PGD_FILE_ALIGN_OBJECT_END

#else 
/**
 * ファイル書き込みを行うオブジェクトのメモリサイズを指定する
 */
#define PGD_FILE_ALIGN_OBJECT_BEGIN

/**
 * ファイル書き込みを行うオブジェクトのメモリサイズを指定する
 */
#define PGD_FILE_ALIGN_OBJECT_END       __attribute__((aligned(8)))

#endif
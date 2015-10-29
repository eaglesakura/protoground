#pragma once

#include    "es/protoground.hpp"

// 対応しているレンダラが定義される
// #define ES_GRAPHICS_OPENGL    1
// #define ES_GRAPHICS_DIRECTX12 1
// #define ES_GRAPHICS_METAL     1

#if defined(BUILD_Android)
// AndroidはOpenGL ES必須
#define ES_GRAPHICS_OPENGL  1
#endif

#if defined(BUILD_MacOSX)
// Mac OS Xは今のところOpenGL 4.1限定
#define ES_GRAPHICS_OPENGL  1
#endif

#if defined(BUILD_Windows)
#define ES_GRAPHICS_OPENGL  1
#endif

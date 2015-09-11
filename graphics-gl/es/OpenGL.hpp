#pragma once

#include    "es/protoground.hpp"
#include    "es/graphics/Graphics.hpp"

#if !defined(ES_GRAPHICS_OPENGL)
#error Graphics Engine OpenGL not support!!
#endif

// include gl
#if defined(BUILD_MacOSX)

#include    <OpenGL/gl3.h>
#include    <OpenGL/gl3ext.h>

#elif defined(BUILD_Android)

#define __gl2_h_    /* include guard */

#include    <GLES3/gl31.h>
#include    <GLES3/gl3ext.h>
#include    <GLES2/gl2ext.h>
#include    <EGL/egl.h>
#include    <EGL/eglext.h>

#elif defined(BUILD_iOS)
#import     <OpenGLES/ES2/gl.h>
#import     <OpenGLES/ES2/glext.h>
#elif defined(BUILD_Windows_Cygwin)

#include <GL/gl.h>
#endif

#ifndef GL_LUMINANCE
#define GL_LUMINANCE                      0x1909
#endif

#ifndef GL_HALF_FLOAT_OES
#define GL_HALF_FLOAT_OES                 0x8D61
#endif

#ifndef GL_DEPTH_COMPONENT32_OES
#define GL_DEPTH_COMPONENT32_OES          0x81A7
#endif

#ifndef GL_DEPTH_COMPONENT24_OES
#define GL_DEPTH_COMPONENT24_OES          0x81A6
#endif

#ifndef GL_DEPTH24_STENCIL8_OES
#define GL_DEPTH24_STENCIL8_OES           0x88F0
#endif

namespace es {

namespace gl {

namespace internal {
/**
 * 関連付けられたスレッドのglGetErrorを取り出す
 */
GLenum printError(const char *__file__, const int __line__);
}

} /* gl */
} /* es */

/**
 * 緊急時のデバッグ用
 */
//#define NO_GL_ASSERT 1

#if defined(DEBUG) && !defined(NO_GL_ASSERT)
#define     assert_gl(...)    { assert(::es::gl::internal::printError(__FILE__, __LINE__) == GL_NO_ERROR); }
#else
// release
#define     assert_gl(...)    { }
#endif

#if 0

// math
#include    "es/graphics/math/GlmHelper.hpp"

// base
#include    "es/graphics/gl/surface/VirtualDisplay.hpp"
#include    "es/graphics/surface/IRenderingSurface.hpp"
#include    "es/graphics/gl/GLObject.h"
#include    "es/graphics/Color.hpp"
#include    "es/graphics/PixelFormat.hpp"
#include    "es/graphics/gl/context/DeviceContext.h"

// resources
#include    "es/graphics/gl/resource/Texture.h"
#include    "es/graphics/gl/surface/RenderBufferObject.hpp"
#include    "es/graphics/gl/surface/FrameBufferObject.hpp"
#include    "es/graphics/gl/resource/ShaderProgram.h"
#include    "es/graphics/gl/resource/VertexBufferObject.hpp"
#include    "es/graphics/gl/resource/IndexBufferObject.hpp"

// shader
#include    "es/graphics/gl/shader/UniformBase.hpp"
#include    "es/graphics/gl/shader/VectorUniform.hpp"
#include    "es/graphics/gl/shader/ColorUniform.hpp"
#include    "es/graphics/gl/shader/TextureUniform.hpp"
#include    "es/graphics/gl/shader/VertexAttribute.hpp"

#endif
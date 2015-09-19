#pragma once

#include    "es/protoground.hpp"
#include    "es/graphics/Graphics.hpp"

#if !defined(ES_GRAPHICS_OPENGL)
#error Graphics Engine OpenGL not support!!
#endif

/**
 * OpenGL Functions
 */
#include "es/gl-compat.h"


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
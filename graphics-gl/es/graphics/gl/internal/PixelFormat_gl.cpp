#include "es/graphics/PixelFormat.hpp"
#include "es/OpenGL.hpp"

namespace es {


uint32_t Pixel::toGLPixelDataType(const PixelFormat_e format) {
    constexpr const GLenum PIXEL_TYPES[] = {
            //
            GL_UNSIGNED_SHORT_5_6_5,// RGB565
            GL_UNSIGNED_SHORT_5_5_5_1, // RGBA5551
            GL_UNSIGNED_BYTE, // RGB888
            GL_UNSIGNED_BYTE, // RGBA8888
            GL_UNSIGNED_BYTE, // BGRA8888
            GL_HALF_FLOAT_OES, // LuminanceF16
            GL_HALF_FLOAT_OES, // RGBA F16
            GL_UNSIGNED_BYTE, // A8
            GL_UNSIGNED_SHORT, // Depth
            GL_UNSIGNED_BYTE, // Luminance8
            GL_UNSIGNED_BYTE, // R8
            //
    };
    return PIXEL_TYPES[format];
}

uint32_t  Pixel::toGLPixelFormat(const PixelFormat_e format) {
    static const GLenum PIXEL_FORMATS[] = {
            //
            GL_RGB,//   RGB565
            GL_RGBA,  //    RGBA5551
            GL_RGB,  // RGB888
            GL_RGBA, // RGBA8888
#ifdef GL_BGRA_EXT
            GL_BGRA_EXT, // BGRA8888
#else
            GL_RGBA, // RGBA8888
#endif
            GL_LUMINANCE, // L-16
            GL_RGBA, // RGBA F16
            GL_ALPHA, // A8
            GL_DEPTH_COMPONENT, // Depth
            GL_LUMINANCE, // L8
            GL_RED, // R8
            //
    };
    return PIXEL_FORMATS[format];
}

}
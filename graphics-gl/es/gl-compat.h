/*
** Copyright (c) 2013-2014 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/
/*
** This header is generated from the Khronos OpenGL / OpenGL ES XML
** API Registry. The current version of the Registry, generator scripts
** used to make the header, and the header can be found at
**   http://www.opengl.org/registry/
**
** Khronos $Revision: 26696 $ on $Date: 2014-05-17 14:48:55 -0700 (Sat, 17 May 2014) $
*/
#pragma once
#ifndef     __PRG_GL_COMPAT_H__

#if defined(__MACH__)

#include <OpenGL/gltypes.h>

#elif defined(__ANDROID__)
#include <KHR/khrplatform.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*pgdGetGlProcAddress_ptr)(void *pUserData, char *procName);

/**
 * OpenGL関数ポインタの初期化を行う
 */
extern void pgdGlCompatInitialize(pgdGetGlProcAddress_ptr getProcAddress, void *pUserData);

typedef enum ePgdGraphicsOpenGlCompat_e {
    PgdGraphicsOpenGlCompat_Unknown,
    PgdGraphicsOpenGlCompat_ES20,
    PgdGraphicsOpenGlCompat_ES30,
    PgdGraphicsOpenGlCompat_ES31,
    PgdGraphicsOpenGlCompat_ES31_AEP,
} PgdGraphicsOpenGlCompat_e;

/**
 * OpenGL互換バージョンを取得する
 */
extern PgdGraphicsOpenGlCompat_e pgdGlGetCompatVersion();

#define     PGD_OPENGL_FUNCTION(ret, name)     extern  ret (*name)

/******************************************************************************/
/*                         OpenGL ES 2.0 compat                               */
/******************************************************************************/
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
#define GL_FUNC_ADD                       0x8006
#define GL_BLEND_EQUATION                 0x8009
#define GL_BLEND_EQUATION_RGB             0x8009
#define GL_BLEND_EQUATION_ALPHA           0x883D
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_FUNC_REVERSE_SUBTRACT          0x800B
#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_SRC_ALPHA                0x80CB
#define GL_CONSTANT_COLOR                 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define GL_BLEND_COLOR                    0x8005
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_BUFFER_SIZE                    0x8764
#define GL_BUFFER_USAGE                   0x8765
#define GL_CURRENT_VERTEX_ATTRIB          0x8626
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_FRONT_AND_BACK                 0x0408
#define GL_TEXTURE_2D                     0x0DE1
#define GL_CULL_FACE                      0x0B44
#define GL_BLEND                          0x0BE2
#define GL_DITHER                         0x0BD0
#define GL_STENCIL_TEST                   0x0B90
#define GL_DEPTH_TEST                     0x0B71
#define GL_SCISSOR_TEST                   0x0C11
#define GL_POLYGON_OFFSET_FILL            0x8037
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_OUT_OF_MEMORY                  0x0505
#define GL_CW                             0x0900
#define GL_CCW                            0x0901
#define GL_LINE_WIDTH                     0x0B21
#define GL_ALIASED_POINT_SIZE_RANGE       0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E
#define GL_CULL_FACE_MODE                 0x0B45
#define GL_FRONT_FACE                     0x0B46
#define GL_DEPTH_RANGE                    0x0B70
#define GL_DEPTH_WRITEMASK                0x0B72
#define GL_DEPTH_CLEAR_VALUE              0x0B73
#define GL_DEPTH_FUNC                     0x0B74
#define GL_STENCIL_CLEAR_VALUE            0x0B91
#define GL_STENCIL_FUNC                   0x0B92
#define GL_STENCIL_FAIL                   0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS        0x0B96
#define GL_STENCIL_REF                    0x0B97
#define GL_STENCIL_VALUE_MASK             0x0B93
#define GL_STENCIL_WRITEMASK              0x0B98
#define GL_STENCIL_BACK_FUNC              0x8800
#define GL_STENCIL_BACK_FAIL              0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define GL_STENCIL_BACK_REF               0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK        0x8CA4
#define GL_STENCIL_BACK_WRITEMASK         0x8CA5
#define GL_VIEWPORT                       0x0BA2
#define GL_SCISSOR_BOX                    0x0C10
#define GL_COLOR_CLEAR_VALUE              0x0C22
#define GL_COLOR_WRITEMASK                0x0C23
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_VIEWPORT_DIMS              0x0D3A
#define GL_SUBPIXEL_BITS                  0x0D50
#define GL_RED_BITS                       0x0D52
#define GL_GREEN_BITS                     0x0D53
#define GL_BLUE_BITS                      0x0D54
#define GL_ALPHA_BITS                     0x0D55
#define GL_DEPTH_BITS                     0x0D56
#define GL_STENCIL_BITS                   0x0D57
#define GL_POLYGON_OFFSET_UNITS           0x2A00
#define GL_POLYGON_OFFSET_FACTOR          0x8038
#define GL_TEXTURE_BINDING_2D             0x8069
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define GL_DONT_CARE                      0x1100
#define GL_FASTEST                        0x1101
#define GL_NICEST                         0x1102
#define GL_GENERATE_MIPMAP_HINT           0x8192
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_FIXED                          0x140C
#define GL_DEPTH_COMPONENT                0x1902
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_MAX_VERTEX_UNIFORM_VECTORS     0x8DFB
#define GL_MAX_VARYING_VECTORS            0x8DFC
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS   0x8DFD
#define GL_SHADER_TYPE                    0x8B4F
#define GL_DELETE_STATUS                  0x8B80
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NOTEQUAL                       0x0205
#define GL_GEQUAL                         0x0206
#define GL_ALWAYS                         0x0207
#define GL_KEEP                           0x1E00
#define GL_REPLACE                        0x1E01
#define GL_INCR                           0x1E02
#define GL_DECR                           0x1E03
#define GL_INVERT                         0x150A
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_TEXTURE                        0x1702
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_MIRRORED_REPEAT                0x8370
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_BOOL                           0x8B56
#define GL_BOOL_VEC2                      0x8B57
#define GL_BOOL_VEC3                      0x8B58
#define GL_BOOL_VEC4                      0x8B59
#define GL_FLOAT_MAT2                     0x8B5A
#define GL_FLOAT_MAT3                     0x8B5B
#define GL_FLOAT_MAT4                     0x8B5C
#define GL_SAMPLER_2D                     0x8B5E
#define GL_SAMPLER_CUBE                   0x8B60
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_COMPILE_STATUS                 0x8B81
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_SHADER_SOURCE_LENGTH           0x8B88
#define GL_SHADER_COMPILER                0x8DFA
#define GL_SHADER_BINARY_FORMATS          0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS      0x8DF9
#define GL_LOW_FLOAT                      0x8DF0
#define GL_MEDIUM_FLOAT                   0x8DF1
#define GL_HIGH_FLOAT                     0x8DF2
#define GL_LOW_INT                        0x8DF3
#define GL_MEDIUM_INT                     0x8DF4
#define GL_HIGH_INT                       0x8DF5
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_RGBA4                          0x8056
#define GL_RGB5_A1                        0x8057
#define GL_RGB565                         0x8D62
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_RENDERBUFFER_WIDTH             0x8D42
#define GL_RENDERBUFFER_HEIGHT            0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   0x8D44
#define GL_RENDERBUFFER_RED_SIZE          0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE        0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE         0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE        0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE        0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE      0x8D55
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_NONE                           0
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_MAX_RENDERBUFFER_SIZE          0x84E8
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506

PGD_OPENGL_FUNCTION(void, glActiveTexture)(GLenum texture);

PGD_OPENGL_FUNCTION(void, glAttachShader)(GLuint program, GLuint shader);

PGD_OPENGL_FUNCTION(void, glBindAttribLocation)(GLuint program, GLuint index, const GLchar *name);

PGD_OPENGL_FUNCTION(void, glBindBuffer)(GLenum target, GLuint buffer);

PGD_OPENGL_FUNCTION(void, glBindFramebuffer)(GLenum target, GLuint framebuffer);

PGD_OPENGL_FUNCTION(void, glBindRenderbuffer)(GLenum target, GLuint renderbuffer);

PGD_OPENGL_FUNCTION(void, glBindTexture)(GLenum target, GLuint texture);

PGD_OPENGL_FUNCTION(void, glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

PGD_OPENGL_FUNCTION(void, glBlendEquation)(GLenum mode);

PGD_OPENGL_FUNCTION(void, glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);

PGD_OPENGL_FUNCTION(void, glBlendFunc)(GLenum sfactor, GLenum dfactor);

PGD_OPENGL_FUNCTION(void, glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

PGD_OPENGL_FUNCTION(void, glBufferData)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);

PGD_OPENGL_FUNCTION(void, glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);

PGD_OPENGL_FUNCTION(GLenum, glCheckFramebufferStatus)(GLenum target);

PGD_OPENGL_FUNCTION(void, glClear)(GLbitfield mask);

PGD_OPENGL_FUNCTION(void, glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

PGD_OPENGL_FUNCTION(void, glClearDepthf)(GLfloat d);

PGD_OPENGL_FUNCTION(void, glClearStencil)(GLint s);

PGD_OPENGL_FUNCTION(void, glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

PGD_OPENGL_FUNCTION(void, glCompileShader)(GLuint shader);

PGD_OPENGL_FUNCTION(void, glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);

PGD_OPENGL_FUNCTION(void, glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);

PGD_OPENGL_FUNCTION(void, glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);

PGD_OPENGL_FUNCTION(void, glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

PGD_OPENGL_FUNCTION(GLuint, glCreateProgram)(void);

PGD_OPENGL_FUNCTION(GLuint, glCreateShader)(GLenum type);

PGD_OPENGL_FUNCTION(void, glCullFace)(GLenum mode);

PGD_OPENGL_FUNCTION(void, glDeleteBuffers)(GLsizei n, const GLuint *buffers);

PGD_OPENGL_FUNCTION(void, glDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);

PGD_OPENGL_FUNCTION(void, glDeleteProgram)(GLuint program);

PGD_OPENGL_FUNCTION(void, glDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);

PGD_OPENGL_FUNCTION(void, glDeleteShader)(GLuint shader);

PGD_OPENGL_FUNCTION(void, glDeleteTextures)(GLsizei n, const GLuint *textures);

PGD_OPENGL_FUNCTION(void, glDepthFunc)(GLenum func);

PGD_OPENGL_FUNCTION(void, glDepthMask)(GLboolean flag);

PGD_OPENGL_FUNCTION(void, glDepthRangef)(GLfloat n, GLfloat f);

PGD_OPENGL_FUNCTION(void, glDetachShader)(GLuint program, GLuint shader);

PGD_OPENGL_FUNCTION(void, glDisable)(GLenum cap);

PGD_OPENGL_FUNCTION(void, glDisableVertexAttribArray)(GLuint index);

PGD_OPENGL_FUNCTION(void, glDrawArrays)(GLenum mode, GLint first, GLsizei count);

PGD_OPENGL_FUNCTION(void, glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices);

PGD_OPENGL_FUNCTION(void, glEnable)(GLenum cap);

PGD_OPENGL_FUNCTION(void, glEnableVertexAttribArray)(GLuint index);

PGD_OPENGL_FUNCTION(void, glFinish)(void);

PGD_OPENGL_FUNCTION(void, glFlush)(void);

PGD_OPENGL_FUNCTION(void, glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

PGD_OPENGL_FUNCTION(void, glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

PGD_OPENGL_FUNCTION(void, glFrontFace)(GLenum mode);

PGD_OPENGL_FUNCTION(void, glGenBuffers)(GLsizei n, GLuint *buffers);

PGD_OPENGL_FUNCTION(void, glGenerateMipmap)(GLenum target);

PGD_OPENGL_FUNCTION(void, glGenFramebuffers)(GLsizei n, GLuint *framebuffers);

PGD_OPENGL_FUNCTION(void, glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers);

PGD_OPENGL_FUNCTION(void, glGenTextures)(GLsizei n, GLuint *textures);

PGD_OPENGL_FUNCTION(void, glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);

PGD_OPENGL_FUNCTION(void, glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);

PGD_OPENGL_FUNCTION(void, glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);

PGD_OPENGL_FUNCTION(GLint, glGetAttribLocation)(GLuint program, const GLchar *name);

PGD_OPENGL_FUNCTION(void, glGetBooleanv)(GLenum pname, GLboolean *data);

PGD_OPENGL_FUNCTION(void, glGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(GLenum, glGetError)(void);

PGD_OPENGL_FUNCTION(void, glGetFloatv)(GLenum pname, GLfloat *data);

PGD_OPENGL_FUNCTION(void, glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetIntegerv)(GLenum pname, GLint *data);

PGD_OPENGL_FUNCTION(void, glGetProgramiv)(GLuint program, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

PGD_OPENGL_FUNCTION(void, glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetShaderiv)(GLuint shader, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

PGD_OPENGL_FUNCTION(void, glGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);

PGD_OPENGL_FUNCTION(void, glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);

PGD_OPENGL_FUNCTION(const GLubyte *, glGetString)(GLenum name);

PGD_OPENGL_FUNCTION(void, glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);

PGD_OPENGL_FUNCTION(void, glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetUniformfv)(GLuint program, GLint location, GLfloat *params);

PGD_OPENGL_FUNCTION(void, glGetUniformiv)(GLuint program, GLint location, GLint *params);

PGD_OPENGL_FUNCTION(GLint, glGetUniformLocation)(GLuint program, const GLchar *name);

PGD_OPENGL_FUNCTION(void, glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params);

PGD_OPENGL_FUNCTION(void, glGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetVertexAttribPointerv)(GLuint index, GLenum pname, void **pointer);

PGD_OPENGL_FUNCTION(void, glHint)(GLenum target, GLenum mode);

PGD_OPENGL_FUNCTION(GLboolean, glIsBuffer)(GLuint buffer);

PGD_OPENGL_FUNCTION(GLboolean, glIsEnabled)(GLenum cap);

PGD_OPENGL_FUNCTION(GLboolean, glIsFramebuffer)(GLuint framebuffer);

PGD_OPENGL_FUNCTION(GLboolean, glIsProgram)(GLuint program);

PGD_OPENGL_FUNCTION(GLboolean, glIsRenderbuffer)(GLuint renderbuffer);

PGD_OPENGL_FUNCTION(GLboolean, glIsShader)(GLuint shader);

PGD_OPENGL_FUNCTION(GLboolean, glIsTexture)(GLuint texture);

PGD_OPENGL_FUNCTION(void, glLineWidth)(GLfloat width);

PGD_OPENGL_FUNCTION(void, glLinkProgram)(GLuint program);

PGD_OPENGL_FUNCTION(void, glPixelStorei)(GLenum pname, GLint param);

PGD_OPENGL_FUNCTION(void, glPolygonOffset)(GLfloat factor, GLfloat units);

PGD_OPENGL_FUNCTION(void, glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);

PGD_OPENGL_FUNCTION(void, glReleaseShaderCompiler)(void);

PGD_OPENGL_FUNCTION(void, glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

PGD_OPENGL_FUNCTION(void, glSampleCoverage)(GLfloat value, GLboolean invert);

PGD_OPENGL_FUNCTION(void, glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);

PGD_OPENGL_FUNCTION(void, glShaderBinary)(GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);

PGD_OPENGL_FUNCTION(void, glShaderSource)(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length);

PGD_OPENGL_FUNCTION(void, glStencilFunc)(GLenum func, GLint ref, GLuint mask);

PGD_OPENGL_FUNCTION(void, glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);

PGD_OPENGL_FUNCTION(void, glStencilMask)(GLuint mask);

PGD_OPENGL_FUNCTION(void, glStencilMaskSeparate)(GLenum face, GLuint mask);

PGD_OPENGL_FUNCTION(void, glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);

PGD_OPENGL_FUNCTION(void, glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);

PGD_OPENGL_FUNCTION(void, glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);

PGD_OPENGL_FUNCTION(void, glTexParameterf)(GLenum target, GLenum pname, GLfloat param);

PGD_OPENGL_FUNCTION(void, glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);

PGD_OPENGL_FUNCTION(void, glTexParameteri)(GLenum target, GLenum pname, GLint param);

PGD_OPENGL_FUNCTION(void, glTexParameteriv)(GLenum target, GLenum pname, const GLint *params);

PGD_OPENGL_FUNCTION(void, glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);

PGD_OPENGL_FUNCTION(void, glUniform1f)(GLint location, GLfloat v0);

PGD_OPENGL_FUNCTION(void, glUniform1fv)(GLint location, GLsizei count, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUniform1i)(GLint location, GLint v0);

PGD_OPENGL_FUNCTION(void, glUniform1iv)(GLint location, GLsizei count, const GLint *value);

PGD_OPENGL_FUNCTION(void, glUniform2f)(GLint location, GLfloat v0, GLfloat v1);

PGD_OPENGL_FUNCTION(void, glUniform2fv)(GLint location, GLsizei count, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUniform2i)(GLint location, GLint v0, GLint v1);

PGD_OPENGL_FUNCTION(void, glUniform2iv)(GLint location, GLsizei count, const GLint *value);

PGD_OPENGL_FUNCTION(void, glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);

PGD_OPENGL_FUNCTION(void, glUniform3fv)(GLint location, GLsizei count, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2);

PGD_OPENGL_FUNCTION(void, glUniform3iv)(GLint location, GLsizei count, const GLint *value);

PGD_OPENGL_FUNCTION(void, glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

PGD_OPENGL_FUNCTION(void, glUniform4fv)(GLint location, GLsizei count, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

PGD_OPENGL_FUNCTION(void, glUniform4iv)(GLint location, GLsizei count, const GLint *value);

PGD_OPENGL_FUNCTION(void, glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUseProgram)(GLuint program);

PGD_OPENGL_FUNCTION(void, glValidateProgram)(GLuint program);

PGD_OPENGL_FUNCTION(void, glVertexAttrib1f)(GLuint index, GLfloat x);

PGD_OPENGL_FUNCTION(void, glVertexAttrib1fv)(GLuint index, const GLfloat *v);

PGD_OPENGL_FUNCTION(void, glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);

PGD_OPENGL_FUNCTION(void, glVertexAttrib2fv)(GLuint index, const GLfloat *v);

PGD_OPENGL_FUNCTION(void, glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);

PGD_OPENGL_FUNCTION(void, glVertexAttrib3fv)(GLuint index, const GLfloat *v);

PGD_OPENGL_FUNCTION(void, glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

PGD_OPENGL_FUNCTION(void, glVertexAttrib4fv)(GLuint index, const GLfloat *v);

PGD_OPENGL_FUNCTION(void, glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

PGD_OPENGL_FUNCTION(void, glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);

/******************************************************************************/
/*                         OpenGL ES 3.0 compat                               */
/******************************************************************************/
typedef unsigned short GLhalf;
#define GL_READ_BUFFER                    0x0C02
#define GL_UNPACK_ROW_LENGTH              0x0CF2
#define GL_UNPACK_SKIP_ROWS               0x0CF3
#define GL_UNPACK_SKIP_PIXELS             0x0CF4
#define GL_PACK_ROW_LENGTH                0x0D02
#define GL_PACK_SKIP_ROWS                 0x0D03
#define GL_PACK_SKIP_PIXELS               0x0D04
#define GL_COLOR                          0x1800
#define GL_DEPTH                          0x1801
#define GL_STENCIL                        0x1802
#define GL_RED                            0x1903
#define GL_RGB8                           0x8051
#define GL_RGBA8                          0x8058
#define GL_RGB10_A2                       0x8059
#define GL_TEXTURE_BINDING_3D             0x806A
#define GL_UNPACK_SKIP_IMAGES             0x806D
#define GL_UNPACK_IMAGE_HEIGHT            0x806E
#define GL_TEXTURE_3D                     0x806F
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_MAX_3D_TEXTURE_SIZE            0x8073
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368
#define GL_MAX_ELEMENTS_VERTICES          0x80E8
#define GL_MAX_ELEMENTS_INDICES           0x80E9
#define GL_TEXTURE_MIN_LOD                0x813A
#define GL_TEXTURE_MAX_LOD                0x813B
#define GL_TEXTURE_BASE_LEVEL             0x813C
#define GL_TEXTURE_MAX_LEVEL              0x813D
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_MAX_TEXTURE_LOD_BIAS           0x84FD
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884D
#define GL_CURRENT_QUERY                  0x8865
#define GL_QUERY_RESULT                   0x8866
#define GL_QUERY_RESULT_AVAILABLE         0x8867
#define GL_BUFFER_MAPPED                  0x88BC
#define GL_BUFFER_MAP_POINTER             0x88BD
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA
#define GL_MAX_DRAW_BUFFERS               0x8824
#define GL_DRAW_BUFFER0                   0x8825
#define GL_DRAW_BUFFER1                   0x8826
#define GL_DRAW_BUFFER2                   0x8827
#define GL_DRAW_BUFFER3                   0x8828
#define GL_DRAW_BUFFER4                   0x8829
#define GL_DRAW_BUFFER5                   0x882A
#define GL_DRAW_BUFFER6                   0x882B
#define GL_DRAW_BUFFER7                   0x882C
#define GL_DRAW_BUFFER8                   0x882D
#define GL_DRAW_BUFFER9                   0x882E
#define GL_DRAW_BUFFER10                  0x882F
#define GL_DRAW_BUFFER11                  0x8830
#define GL_DRAW_BUFFER12                  0x8831
#define GL_DRAW_BUFFER13                  0x8832
#define GL_DRAW_BUFFER14                  0x8833
#define GL_DRAW_BUFFER15                  0x8834
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS  0x8B4A
#define GL_SAMPLER_3D                     0x8B5F
#define GL_SAMPLER_2D_SHADOW              0x8B62
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_PIXEL_PACK_BUFFER              0x88EB
#define GL_PIXEL_UNPACK_BUFFER            0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING      0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING    0x88EF
#define GL_FLOAT_MAT2x3                   0x8B65
#define GL_FLOAT_MAT2x4                   0x8B66
#define GL_FLOAT_MAT3x2                   0x8B67
#define GL_FLOAT_MAT3x4                   0x8B68
#define GL_FLOAT_MAT4x2                   0x8B69
#define GL_FLOAT_MAT4x3                   0x8B6A
#define GL_SRGB                           0x8C40
#define GL_SRGB8                          0x8C41
#define GL_SRGB8_ALPHA8                   0x8C43
#define GL_COMPARE_REF_TO_TEXTURE         0x884E
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_NUM_EXTENSIONS                 0x821D
#define GL_RGBA32F                        0x8814
#define GL_RGB32F                         0x8815
#define GL_RGBA16F                        0x881A
#define GL_RGB16F                         0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER    0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS       0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET       0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET       0x8905
#define GL_MAX_VARYING_COMPONENTS         0x8B4B
#define GL_TEXTURE_2D_ARRAY               0x8C1A
#define GL_TEXTURE_BINDING_2D_ARRAY       0x8C1D
#define GL_R11F_G11F_B10F                 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV   0x8C3B
#define GL_RGB9_E5                        0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV       0x8C3E
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS    0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD             0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS            0x8C8C
#define GL_SEPARATE_ATTRIBS               0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER      0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI                       0x8D70
#define GL_RGB32UI                        0x8D71
#define GL_RGBA16UI                       0x8D76
#define GL_RGB16UI                        0x8D77
#define GL_RGBA8UI                        0x8D7C
#define GL_RGB8UI                         0x8D7D
#define GL_RGBA32I                        0x8D82
#define GL_RGB32I                         0x8D83
#define GL_RGBA16I                        0x8D88
#define GL_RGB16I                         0x8D89
#define GL_RGBA8I                         0x8D8E
#define GL_RGB8I                          0x8D8F
#define GL_RED_INTEGER                    0x8D94
#define GL_RGB_INTEGER                    0x8D98
#define GL_RGBA_INTEGER                   0x8D99
#define GL_SAMPLER_2D_ARRAY               0x8DC1
#define GL_SAMPLER_2D_ARRAY_SHADOW        0x8DC4
#define GL_SAMPLER_CUBE_SHADOW            0x8DC5
#define GL_UNSIGNED_INT_VEC2              0x8DC6
#define GL_UNSIGNED_INT_VEC3              0x8DC7
#define GL_UNSIGNED_INT_VEC4              0x8DC8
#define GL_INT_SAMPLER_2D                 0x8DCA
#define GL_INT_SAMPLER_3D                 0x8DCB
#define GL_INT_SAMPLER_CUBE               0x8DCC
#define GL_INT_SAMPLER_2D_ARRAY           0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_2D        0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D        0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE      0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY  0x8DD7
#define GL_BUFFER_ACCESS_FLAGS            0x911F
#define GL_BUFFER_MAP_LENGTH              0x9120
#define GL_BUFFER_MAP_OFFSET              0x9121
#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_DEPTH32F_STENCIL8              0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT            0x8218
#define GL_FRAMEBUFFER_UNDEFINED          0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_UNSIGNED_NORMALIZED            0x8C17
#define GL_DRAW_FRAMEBUFFER_BINDING       0x8CA6
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING       0x8CAA
#define GL_RENDERBUFFER_SAMPLES           0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_MAX_COLOR_ATTACHMENTS          0x8CDF
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
#define GL_COLOR_ATTACHMENT4              0x8CE4
#define GL_COLOR_ATTACHMENT5              0x8CE5
#define GL_COLOR_ATTACHMENT6              0x8CE6
#define GL_COLOR_ATTACHMENT7              0x8CE7
#define GL_COLOR_ATTACHMENT8              0x8CE8
#define GL_COLOR_ATTACHMENT9              0x8CE9
#define GL_COLOR_ATTACHMENT10             0x8CEA
#define GL_COLOR_ATTACHMENT11             0x8CEB
#define GL_COLOR_ATTACHMENT12             0x8CEC
#define GL_COLOR_ATTACHMENT13             0x8CED
#define GL_COLOR_ATTACHMENT14             0x8CEE
#define GL_COLOR_ATTACHMENT15             0x8CEF
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES                    0x8D57
#define GL_HALF_FLOAT                     0x140B
#define GL_MAP_READ_BIT                   0x0001
#define GL_MAP_WRITE_BIT                  0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT       0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT      0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT         0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT         0x0020
#define GL_RG                             0x8227
#define GL_RG_INTEGER                     0x8228
#define GL_R8                             0x8229
#define GL_RG8                            0x822B
#define GL_R16F                           0x822D
#define GL_R32F                           0x822E
#define GL_RG16F                          0x822F
#define GL_RG32F                          0x8230
#define GL_R8I                            0x8231
#define GL_R8UI                           0x8232
#define GL_R16I                           0x8233
#define GL_R16UI                          0x8234
#define GL_R32I                           0x8235
#define GL_R32UI                          0x8236
#define GL_RG8I                           0x8237
#define GL_RG8UI                          0x8238
#define GL_RG16I                          0x8239
#define GL_RG16UI                         0x823A
#define GL_RG32I                          0x823B
#define GL_RG32UI                         0x823C
#define GL_VERTEX_ARRAY_BINDING           0x85B5
#define GL_R8_SNORM                       0x8F94
#define GL_RG8_SNORM                      0x8F95
#define GL_RGB8_SNORM                     0x8F96
#define GL_RGBA8_SNORM                    0x8F97
#define GL_SIGNED_NORMALIZED              0x8F9C
#define GL_PRIMITIVE_RESTART_FIXED_INDEX  0x8D69
#define GL_COPY_READ_BUFFER               0x8F36
#define GL_COPY_WRITE_BUFFER              0x8F37
#define GL_COPY_READ_BUFFER_BINDING       0x8F36
#define GL_COPY_WRITE_BUFFER_BINDING      0x8F37
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_UNIFORM_BUFFER_BINDING         0x8A28
#define GL_UNIFORM_BUFFER_START           0x8A29
#define GL_UNIFORM_BUFFER_SIZE            0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS      0x8A2B
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS    0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS    0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS    0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE         0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS          0x8A36
#define GL_UNIFORM_TYPE                   0x8A37
#define GL_UNIFORM_SIZE                   0x8A38
#define GL_UNIFORM_NAME_LENGTH            0x8A39
#define GL_UNIFORM_BLOCK_INDEX            0x8A3A
#define GL_UNIFORM_OFFSET                 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE           0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE          0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR           0x8A3E
#define GL_UNIFORM_BLOCK_BINDING          0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE        0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH      0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS  0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX                  0xFFFFFFFFu
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS   0x9122
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS  0x9125
#define GL_MAX_SERVER_WAIT_TIMEOUT        0x9111
#define GL_OBJECT_TYPE                    0x9112
#define GL_SYNC_CONDITION                 0x9113
#define GL_SYNC_STATUS                    0x9114
#define GL_SYNC_FLAGS                     0x9115
#define GL_SYNC_FENCE                     0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE     0x9117
#define GL_UNSIGNALED                     0x9118
#define GL_SIGNALED                       0x9119
#define GL_ALREADY_SIGNALED               0x911A
#define GL_TIMEOUT_EXPIRED                0x911B
#define GL_CONDITION_SATISFIED            0x911C
#define GL_WAIT_FAILED                    0x911D
#define GL_SYNC_FLUSH_COMMANDS_BIT        0x00000001
#define GL_TIMEOUT_IGNORED                0xFFFFFFFFFFFFFFFFull
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR    0x88FE
#define GL_ANY_SAMPLES_PASSED             0x8C2F
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A
#define GL_SAMPLER_BINDING                0x8919
#define GL_RGB10_A2UI                     0x906F
#define GL_TEXTURE_SWIZZLE_R              0x8E42
#define GL_TEXTURE_SWIZZLE_G              0x8E43
#define GL_TEXTURE_SWIZZLE_B              0x8E44
#define GL_TEXTURE_SWIZZLE_A              0x8E45
#define GL_GREEN                          0x1904
#define GL_BLUE                           0x1905
#define GL_INT_2_10_10_10_REV             0x8D9F
#define GL_TRANSFORM_FEEDBACK             0x8E22
#define GL_TRANSFORM_FEEDBACK_PAUSED      0x8E23
#define GL_TRANSFORM_FEEDBACK_ACTIVE      0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING     0x8E25
#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_PROGRAM_BINARY_LENGTH          0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS     0x87FE
#define GL_PROGRAM_BINARY_FORMATS         0x87FF
#define GL_COMPRESSED_R11_EAC             0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC      0x9271
#define GL_COMPRESSED_RG11_EAC            0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC     0x9273
#define GL_COMPRESSED_RGB8_ETC2           0x9274
#define GL_COMPRESSED_SRGB8_ETC2          0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC      0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279
#define GL_TEXTURE_IMMUTABLE_FORMAT       0x912F
#define GL_MAX_ELEMENT_INDEX              0x8D6B
#define GL_NUM_SAMPLE_COUNTS              0x9380
#define GL_TEXTURE_IMMUTABLE_LEVELS       0x82DF

PGD_OPENGL_FUNCTION(void, glReadBuffer)(GLenum mode);

PGD_OPENGL_FUNCTION(void, glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);

PGD_OPENGL_FUNCTION(void, glTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);

PGD_OPENGL_FUNCTION(void, glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);

PGD_OPENGL_FUNCTION(void, glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

PGD_OPENGL_FUNCTION(void, glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);

PGD_OPENGL_FUNCTION(void, glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);

PGD_OPENGL_FUNCTION(void, glGenQueries)(GLsizei n, GLuint *ids);

PGD_OPENGL_FUNCTION(void, glDeleteQueries)(GLsizei n, const GLuint *ids);

PGD_OPENGL_FUNCTION(GLboolean, glIsQuery)(GLuint id);

PGD_OPENGL_FUNCTION(void, glBeginQuery)(GLenum target, GLuint id);

PGD_OPENGL_FUNCTION(void, glEndQuery)(GLenum target);

PGD_OPENGL_FUNCTION(void, glGetQueryiv)(GLenum target, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);

PGD_OPENGL_FUNCTION(GLboolean, glUnmapBuffer)(GLenum target);

PGD_OPENGL_FUNCTION(void, glGetBufferPointerv)(GLenum target, GLenum pname, void **params);

PGD_OPENGL_FUNCTION(void, glDrawBuffers)(GLsizei n, const GLenum *bufs);

PGD_OPENGL_FUNCTION(void, glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

PGD_OPENGL_FUNCTION(void, glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

PGD_OPENGL_FUNCTION(void, glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

PGD_OPENGL_FUNCTION(void *, glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);

PGD_OPENGL_FUNCTION(void, glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);

PGD_OPENGL_FUNCTION(void, glBindVertexArray)(GLuint array);

PGD_OPENGL_FUNCTION(void, glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);

PGD_OPENGL_FUNCTION(void, glGenVertexArrays)(GLsizei n, GLuint *arrays);

PGD_OPENGL_FUNCTION(GLboolean, glIsVertexArray)(GLuint array);

PGD_OPENGL_FUNCTION(void, glGetIntegeri_v)(GLenum target, GLuint index, GLint *data);

PGD_OPENGL_FUNCTION(void, glBeginTransformFeedback)(GLenum primitiveMode);

PGD_OPENGL_FUNCTION(void, glEndTransformFeedback)(void);

PGD_OPENGL_FUNCTION(void, glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);

PGD_OPENGL_FUNCTION(void, glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);

PGD_OPENGL_FUNCTION(void, glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar *const *varyings, GLenum bufferMode);

PGD_OPENGL_FUNCTION(void, glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);

PGD_OPENGL_FUNCTION(void, glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);

PGD_OPENGL_FUNCTION(void, glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);

PGD_OPENGL_FUNCTION(void, glVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);

PGD_OPENGL_FUNCTION(void, glVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);

PGD_OPENGL_FUNCTION(void, glVertexAttribI4iv)(GLuint index, const GLint *v);

PGD_OPENGL_FUNCTION(void, glVertexAttribI4uiv)(GLuint index, const GLuint *v);

PGD_OPENGL_FUNCTION(void, glGetUniformuiv)(GLuint program, GLint location, GLuint *params);

PGD_OPENGL_FUNCTION(GLint, glGetFragDataLocation)(GLuint program, const GLchar *name);

PGD_OPENGL_FUNCTION(void, glUniform1ui)(GLint location, GLuint v0);

PGD_OPENGL_FUNCTION(void, glUniform2ui)(GLint location, GLuint v0, GLuint v1);

PGD_OPENGL_FUNCTION(void, glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);

PGD_OPENGL_FUNCTION(void, glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

PGD_OPENGL_FUNCTION(void, glUniform1uiv)(GLint location, GLsizei count, const GLuint *value);

PGD_OPENGL_FUNCTION(void, glUniform2uiv)(GLint location, GLsizei count, const GLuint *value);

PGD_OPENGL_FUNCTION(void, glUniform3uiv)(GLint location, GLsizei count, const GLuint *value);

PGD_OPENGL_FUNCTION(void, glUniform4uiv)(GLint location, GLsizei count, const GLuint *value);

PGD_OPENGL_FUNCTION(void, glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value);

PGD_OPENGL_FUNCTION(void, glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value);

PGD_OPENGL_FUNCTION(void, glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);

PGD_OPENGL_FUNCTION(const GLubyte *, glGetStringi)(GLenum name, GLuint index);

PGD_OPENGL_FUNCTION(void, glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);

PGD_OPENGL_FUNCTION(void, glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar *const *uniformNames, GLuint *uniformIndices);

PGD_OPENGL_FUNCTION(void, glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(GLuint, glGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName);

PGD_OPENGL_FUNCTION(void, glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);

PGD_OPENGL_FUNCTION(void, glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

PGD_OPENGL_FUNCTION(void, glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);

PGD_OPENGL_FUNCTION(void, glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);

PGD_OPENGL_FUNCTION(GLsync, glFenceSync)(GLenum condition, GLbitfield flags);

PGD_OPENGL_FUNCTION(GLboolean, glIsSync)(GLsync sync);

PGD_OPENGL_FUNCTION(void, glDeleteSync)(GLsync sync);

PGD_OPENGL_FUNCTION(GLenum, glClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);

PGD_OPENGL_FUNCTION(void, glWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);

PGD_OPENGL_FUNCTION(void, glGetInteger64v)(GLenum pname, GLint64 *data);

PGD_OPENGL_FUNCTION(void, glGetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);

PGD_OPENGL_FUNCTION(void, glGetInteger64i_v)(GLenum target, GLuint index, GLint64 *data);

PGD_OPENGL_FUNCTION(void, glGetBufferParameteri64v)(GLenum target, GLenum pname, GLint64 *params);

PGD_OPENGL_FUNCTION(void, glGenSamplers)(GLsizei count, GLuint *samplers);

PGD_OPENGL_FUNCTION(void, glDeleteSamplers)(GLsizei count, const GLuint *samplers);

PGD_OPENGL_FUNCTION(GLboolean, glIsSampler)(GLuint sampler);

PGD_OPENGL_FUNCTION(void, glBindSampler)(GLuint unit, GLuint sampler);

PGD_OPENGL_FUNCTION(void, glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);

PGD_OPENGL_FUNCTION(void, glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *param);

PGD_OPENGL_FUNCTION(void, glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);

PGD_OPENGL_FUNCTION(void, glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *param);

PGD_OPENGL_FUNCTION(void, glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params);

PGD_OPENGL_FUNCTION(void, glVertexAttribDivisor)(GLuint index, GLuint divisor);

PGD_OPENGL_FUNCTION(void, glBindTransformFeedback)(GLenum target, GLuint id);

PGD_OPENGL_FUNCTION(void, glDeleteTransformFeedbacks)(GLsizei n, const GLuint *ids);

PGD_OPENGL_FUNCTION(void, glGenTransformFeedbacks)(GLsizei n, GLuint *ids);

PGD_OPENGL_FUNCTION(GLboolean, glIsTransformFeedback)(GLuint id);

PGD_OPENGL_FUNCTION(void, glPauseTransformFeedback)(void);

PGD_OPENGL_FUNCTION(void, glResumeTransformFeedback)(void);

PGD_OPENGL_FUNCTION(void, glGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);

PGD_OPENGL_FUNCTION(void, glProgramBinary)(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);

PGD_OPENGL_FUNCTION(void, glProgramParameteri)(GLuint program, GLenum pname, GLint value);

PGD_OPENGL_FUNCTION(void, glInvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments);

PGD_OPENGL_FUNCTION(void, glInvalidateSubFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);

PGD_OPENGL_FUNCTION(void, glTexStorage2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);

PGD_OPENGL_FUNCTION(void, glTexStorage3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

PGD_OPENGL_FUNCTION(void, glGetInternalformativ)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);

/******************************************************************************/
/*                         OpenGL ES 3.1 compat                               */
/******************************************************************************/
#define GL_COMPUTE_SHADER                 0x91B9
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS     0x91BB
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS     0x91BD
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS    0x8265
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT   0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE    0x91BF
#define GL_COMPUTE_WORK_GROUP_SIZE        0x8267
#define GL_DISPATCH_INDIRECT_BUFFER       0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING 0x90EF
#define GL_COMPUTE_SHADER_BIT             0x00000020
#define GL_DRAW_INDIRECT_BUFFER           0x8F3F
#define GL_DRAW_INDIRECT_BUFFER_BINDING   0x8F43
#define GL_MAX_UNIFORM_LOCATIONS          0x826E
#define GL_FRAMEBUFFER_DEFAULT_WIDTH      0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT     0x9311
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES    0x9313
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS 0x9314
#define GL_MAX_FRAMEBUFFER_WIDTH          0x9315
#define GL_MAX_FRAMEBUFFER_HEIGHT         0x9316
#define GL_MAX_FRAMEBUFFER_SAMPLES        0x9318
#define GL_UNIFORM                        0x92E1
#define GL_UNIFORM_BLOCK                  0x92E2
#define GL_PROGRAM_INPUT                  0x92E3
#define GL_PROGRAM_OUTPUT                 0x92E4
#define GL_BUFFER_VARIABLE                0x92E5
#define GL_SHADER_STORAGE_BLOCK           0x92E6
#define GL_ATOMIC_COUNTER_BUFFER          0x92C0
#define GL_TRANSFORM_FEEDBACK_VARYING     0x92F4
#define GL_ACTIVE_RESOURCES               0x92F5
#define GL_MAX_NAME_LENGTH                0x92F6
#define GL_MAX_NUM_ACTIVE_VARIABLES       0x92F7
#define GL_NAME_LENGTH                    0x92F9
#define GL_TYPE                           0x92FA
#define GL_ARRAY_SIZE                     0x92FB
#define GL_OFFSET                         0x92FC
#define GL_BLOCK_INDEX                    0x92FD
#define GL_ARRAY_STRIDE                   0x92FE
#define GL_MATRIX_STRIDE                  0x92FF
#define GL_IS_ROW_MAJOR                   0x9300
#define GL_ATOMIC_COUNTER_BUFFER_INDEX    0x9301
#define GL_BUFFER_BINDING                 0x9302
#define GL_BUFFER_DATA_SIZE               0x9303
#define GL_NUM_ACTIVE_VARIABLES           0x9304
#define GL_ACTIVE_VARIABLES               0x9305
#define GL_REFERENCED_BY_VERTEX_SHADER    0x9306
#define GL_REFERENCED_BY_FRAGMENT_SHADER  0x930A
#define GL_REFERENCED_BY_COMPUTE_SHADER   0x930B
#define GL_TOP_LEVEL_ARRAY_SIZE           0x930C
#define GL_TOP_LEVEL_ARRAY_STRIDE         0x930D
#define GL_LOCATION                       0x930E
#define GL_VERTEX_SHADER_BIT              0x00000001
#define GL_FRAGMENT_SHADER_BIT            0x00000002
#define GL_ALL_SHADER_BITS                0xFFFFFFFF
#define GL_PROGRAM_SEPARABLE              0x8258
#define GL_ACTIVE_PROGRAM                 0x8259
#define GL_PROGRAM_PIPELINE_BINDING       0x825A
#define GL_ATOMIC_COUNTER_BUFFER_BINDING  0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_START    0x92C2
#define GL_ATOMIC_COUNTER_BUFFER_SIZE     0x92C3
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS 0x92CC
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS 0x92D0
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS 0x92D1
#define GL_MAX_VERTEX_ATOMIC_COUNTERS     0x92D2
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS   0x92D6
#define GL_MAX_COMBINED_ATOMIC_COUNTERS   0x92D7
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE 0x92D8
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS 0x92DC
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS  0x92D9
#define GL_UNSIGNED_INT_ATOMIC_COUNTER    0x92DB
#define GL_MAX_IMAGE_UNITS                0x8F38
#define GL_MAX_VERTEX_IMAGE_UNIFORMS      0x90CA
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS    0x90CE
#define GL_MAX_COMBINED_IMAGE_UNIFORMS    0x90CF
#define GL_IMAGE_BINDING_NAME             0x8F3A
#define GL_IMAGE_BINDING_LEVEL            0x8F3B
#define GL_IMAGE_BINDING_LAYERED          0x8F3C
#define GL_IMAGE_BINDING_LAYER            0x8F3D
#define GL_IMAGE_BINDING_ACCESS           0x8F3E
#define GL_IMAGE_BINDING_FORMAT           0x906E
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT      0x00000002
#define GL_UNIFORM_BARRIER_BIT            0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT      0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#define GL_COMMAND_BARRIER_BIT            0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT       0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT     0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT      0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT        0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT     0x00001000
#define GL_ALL_BARRIER_BITS               0xFFFFFFFF
#define GL_IMAGE_2D                       0x904D
#define GL_IMAGE_3D                       0x904E
#define GL_IMAGE_CUBE                     0x9050
#define GL_IMAGE_2D_ARRAY                 0x9053
#define GL_INT_IMAGE_2D                   0x9058
#define GL_INT_IMAGE_3D                   0x9059
#define GL_INT_IMAGE_CUBE                 0x905B
#define GL_INT_IMAGE_2D_ARRAY             0x905E
#define GL_UNSIGNED_INT_IMAGE_2D          0x9063
#define GL_UNSIGNED_INT_IMAGE_3D          0x9064
#define GL_UNSIGNED_INT_IMAGE_CUBE        0x9066
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY    0x9069
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE 0x90C7
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE 0x90C8
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS 0x90C9
#define GL_READ_ONLY                      0x88B8
#define GL_WRITE_ONLY                     0x88B9
#define GL_READ_WRITE                     0x88BA
#define GL_SHADER_STORAGE_BUFFER          0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING  0x90D3
#define GL_SHADER_STORAGE_BUFFER_START    0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE     0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE  0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF
#define GL_SHADER_STORAGE_BARRIER_BIT     0x00002000
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_DEPTH_STENCIL_TEXTURE_MODE     0x90EA
#define GL_STENCIL_INDEX                  0x1901
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_SAMPLE_POSITION                0x8E50
#define GL_SAMPLE_MASK                    0x8E51
#define GL_SAMPLE_MASK_VALUE              0x8E52
#define GL_TEXTURE_2D_MULTISAMPLE         0x9100
#define GL_MAX_SAMPLE_MASK_WORDS          0x8E59
#define GL_MAX_COLOR_TEXTURE_SAMPLES      0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES      0x910F
#define GL_MAX_INTEGER_SAMPLES            0x9110
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_SAMPLES                0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_TEXTURE_WIDTH                  0x1000
#define GL_TEXTURE_HEIGHT                 0x1001
#define GL_TEXTURE_DEPTH                  0x8071
#define GL_TEXTURE_INTERNAL_FORMAT        0x1003
#define GL_TEXTURE_RED_SIZE               0x805C
#define GL_TEXTURE_GREEN_SIZE             0x805D
#define GL_TEXTURE_BLUE_SIZE              0x805E
#define GL_TEXTURE_ALPHA_SIZE             0x805F
#define GL_TEXTURE_DEPTH_SIZE             0x884A
#define GL_TEXTURE_STENCIL_SIZE           0x88F1
#define GL_TEXTURE_SHARED_SIZE            0x8C3F
#define GL_TEXTURE_RED_TYPE               0x8C10
#define GL_TEXTURE_GREEN_TYPE             0x8C11
#define GL_TEXTURE_BLUE_TYPE              0x8C12
#define GL_TEXTURE_ALPHA_TYPE             0x8C13
#define GL_TEXTURE_DEPTH_TYPE             0x8C16
#define GL_TEXTURE_COMPRESSED             0x86A1
#define GL_SAMPLER_2D_MULTISAMPLE         0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE     0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_VERTEX_ATTRIB_BINDING          0x82D4
#define GL_VERTEX_ATTRIB_RELATIVE_OFFSET  0x82D5
#define GL_VERTEX_BINDING_DIVISOR         0x82D6
#define GL_VERTEX_BINDING_OFFSET          0x82D7
#define GL_VERTEX_BINDING_STRIDE          0x82D8
#define GL_VERTEX_BINDING_BUFFER          0x8F4F
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D9
#define GL_MAX_VERTEX_ATTRIB_BINDINGS     0x82DA
#define GL_MAX_VERTEX_ATTRIB_STRIDE       0x82E5

PGD_OPENGL_FUNCTION(void, glDispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);

PGD_OPENGL_FUNCTION(void, glDispatchComputeIndirect)(GLintptr indirect);

PGD_OPENGL_FUNCTION(void, glDrawArraysIndirect)(GLenum mode, const void *indirect);

PGD_OPENGL_FUNCTION(void, glDrawElementsIndirect)(GLenum mode, GLenum type, const void *indirect);

PGD_OPENGL_FUNCTION(void, glFramebufferParameteri)(GLenum target, GLenum pname, GLint param);

PGD_OPENGL_FUNCTION(void, glGetFramebufferParameteriv)(GLenum target, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(GLuint, glGetProgramResourceIndex)(GLuint program, GLenum programInterface, const GLchar *name);

PGD_OPENGL_FUNCTION(void, glGetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);

PGD_OPENGL_FUNCTION(void, glGetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);

PGD_OPENGL_FUNCTION(GLint, glGetProgramResourceLocation)(GLuint program, GLenum programInterface, const GLchar *name);

PGD_OPENGL_FUNCTION(void, glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);

PGD_OPENGL_FUNCTION(void, glActiveShaderProgram)(GLuint pipeline, GLuint program);

PGD_OPENGL_FUNCTION(GLuint, glCreateShaderProgramv)(GLenum type, GLsizei count, const GLchar *const *strings);

PGD_OPENGL_FUNCTION(void, glBindProgramPipeline)(GLuint pipeline);

PGD_OPENGL_FUNCTION(void, glDeleteProgramPipelines)(GLsizei n, const GLuint *pipelines);

PGD_OPENGL_FUNCTION(void, glGenProgramPipelines)(GLsizei n, GLuint *pipelines);

PGD_OPENGL_FUNCTION(GLboolean, glIsProgramPipeline)(GLuint pipeline);

PGD_OPENGL_FUNCTION(void, glGetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glProgramUniform1i)(GLuint program, GLint location, GLint v0);

PGD_OPENGL_FUNCTION(void, glProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1);

PGD_OPENGL_FUNCTION(void, glProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);

PGD_OPENGL_FUNCTION(void, glProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

PGD_OPENGL_FUNCTION(void, glProgramUniform1ui)(GLuint program, GLint location, GLuint v0);

PGD_OPENGL_FUNCTION(void, glProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1);

PGD_OPENGL_FUNCTION(void, glProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);

PGD_OPENGL_FUNCTION(void, glProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

PGD_OPENGL_FUNCTION(void, glProgramUniform1f)(GLuint program, GLint location, GLfloat v0);

PGD_OPENGL_FUNCTION(void, glProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);

PGD_OPENGL_FUNCTION(void, glProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);

PGD_OPENGL_FUNCTION(void, glProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

PGD_OPENGL_FUNCTION(void, glProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint *value);

PGD_OPENGL_FUNCTION(void, glProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint *value);

PGD_OPENGL_FUNCTION(void, glProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint *value);

PGD_OPENGL_FUNCTION(void, glProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint *value);

PGD_OPENGL_FUNCTION(void, glProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);

PGD_OPENGL_FUNCTION(void, glProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);

PGD_OPENGL_FUNCTION(void, glProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);

PGD_OPENGL_FUNCTION(void, glProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);

PGD_OPENGL_FUNCTION(void, glProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

PGD_OPENGL_FUNCTION(void, glValidateProgramPipeline)(GLuint pipeline);

PGD_OPENGL_FUNCTION(void, glGetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

PGD_OPENGL_FUNCTION(void, glBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);

PGD_OPENGL_FUNCTION(void, glGetBooleani_v)(GLenum target, GLuint index, GLboolean *data);

PGD_OPENGL_FUNCTION(void, glMemoryBarrier)(GLbitfield barriers);

PGD_OPENGL_FUNCTION(void, glMemoryBarrierByRegion)(GLbitfield barriers);

PGD_OPENGL_FUNCTION(void, glTexStorage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);

PGD_OPENGL_FUNCTION(void, glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val);

PGD_OPENGL_FUNCTION(void, glSampleMaski)(GLuint maskNumber, GLbitfield mask);

PGD_OPENGL_FUNCTION(void, glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);

PGD_OPENGL_FUNCTION(void, glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);

PGD_OPENGL_FUNCTION(void, glBindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);

PGD_OPENGL_FUNCTION(void, glVertexAttribFormat)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);

PGD_OPENGL_FUNCTION(void, glVertexAttribIFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);

PGD_OPENGL_FUNCTION(void, glVertexAttribBinding)(GLuint attribindex, GLuint bindingindex);

PGD_OPENGL_FUNCTION(void, glVertexBindingDivisor)(GLuint bindingindex, GLuint divisor);

/******************************************************************************/
/*                         OpenGL ES Extension compat                         */
/******************************************************************************/
#define GL_TEXTURE_EXTERNAL_OES           0x8D65
#define GL_TEXTURE_BINDING_EXTERNAL_OES   0x8D67
#define GL_REQUIRED_TEXTURE_IMAGE_UNITS_OES 0x8D68
#define GL_SAMPLER_EXTERNAL_OES           0x8D66

#define GL_ETC1_RGB8_OES                  0x8D64

#define GL_DEPTH_COMPONENT24_OES          0x81A6
#define GL_DEPTH_COMPONENT32_OES          0x81A7

#define GL_WRITE_ONLY_OES                 0x88B9
#define GL_BUFFER_ACCESS_OES              0x88BB
#define GL_BUFFER_MAPPED_OES              0x88BC
#define GL_BUFFER_MAP_POINTER_OES         0x88BD

#define GL_DEPTH_STENCIL_OES              0x84F9
#define GL_UNSIGNED_INT_24_8_OES          0x84FA
#define GL_DEPTH24_STENCIL8_OES           0x88F0

#define GL_ALPHA8_OES                     0x803C
#define GL_DEPTH_COMPONENT16_OES          0x81A5
#define GL_LUMINANCE4_ALPHA4_OES          0x8043
#define GL_LUMINANCE8_ALPHA8_OES          0x8045
#define GL_LUMINANCE8_OES                 0x8040
#define GL_RGBA4_OES                      0x8056
#define GL_RGB5_A1_OES                    0x8057
#define GL_RGB565_OES                     0x8D62
#define GL_RGB8_OES                       0x8051
#define GL_RGBA8_OES                      0x8058
#define GL_RGB10_EXT                      0x8052
#define GL_RGB10_A2_EXT                   0x8059

#define GL_TEXTURE_WRAP_R_OES             0x8072
#define GL_TEXTURE_3D_OES                 0x806F
#define GL_TEXTURE_BINDING_3D_OES         0x806A
#define GL_MAX_3D_TEXTURE_SIZE_OES        0x8073
#define GL_SAMPLER_3D_OES                 0x8B5F
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_OES 0x8CD4

#define GL_VERTEX_ARRAY_BINDING_OES       0x85B5

#define GL_BGRA_EXT                       0x80E1
#define GL_BGRA8_EXT                      0x93A1

#define GL_COLOR_EXT                      0x1800
#define GL_DEPTH_EXT                      0x1801
#define GL_STENCIL_EXT                    0x1802

#define GL_GEOMETRY_SHADER_EXT            0x8DD9
#define GL_GEOMETRY_SHADER_BIT_EXT        0x00000004
#define GL_GEOMETRY_LINKED_VERTICES_OUT_EXT 0x8916
#define GL_GEOMETRY_LINKED_INPUT_TYPE_EXT 0x8917
#define GL_GEOMETRY_LINKED_OUTPUT_TYPE_EXT 0x8918
#define GL_GEOMETRY_SHADER_INVOCATIONS_EXT 0x887F
#define GL_LAYER_PROVOKING_VERTEX_EXT     0x825E
#define GL_LINES_ADJACENCY_EXT            0x000A
#define GL_LINE_STRIP_ADJACENCY_EXT       0x000B
#define GL_TRIANGLES_ADJACENCY_EXT        0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY_EXT   0x000D
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT 0x8DDF
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS_EXT 0x8A2C
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS_EXT 0x8A32
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS_EXT 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS_EXT 0x9124
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT 0x8DE1
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS_EXT 0x8E5A
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT 0x8C29
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS_EXT 0x92CF
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS_EXT 0x92D5
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS_EXT 0x90CD
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS_EXT 0x90D7
#define GL_FIRST_VERTEX_CONVENTION_EXT    0x8E4D
#define GL_LAST_VERTEX_CONVENTION_EXT     0x8E4E
#define GL_UNDEFINED_VERTEX_EXT           0x8260
#define GL_PRIMITIVES_GENERATED_EXT       0x8C87
#define GL_FRAMEBUFFER_DEFAULT_LAYERS_EXT 0x9312
#define GL_MAX_FRAMEBUFFER_LAYERS_EXT     0x9317
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT 0x8DA8
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT 0x8DA7
#define GL_REFERENCED_BY_GEOMETRY_SHADER_EXT 0x9309

#define GL_MAP_READ_BIT_EXT               0x0001
#define GL_MAP_WRITE_BIT_EXT              0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT_EXT   0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT_EXT  0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT_EXT     0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT_EXT     0x0020

#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_SAMPLES_EXT 0x8D6C
#define GL_RENDERBUFFER_SAMPLES_EXT       0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT 0x8D56
#define GL_MAX_SAMPLES_EXT                0x8D57

#define GL_PATCHES_EXT                    0x000E
#define GL_PATCH_VERTICES_EXT             0x8E72
#define GL_TESS_CONTROL_OUTPUT_VERTICES_EXT 0x8E75
#define GL_TESS_GEN_MODE_EXT              0x8E76
#define GL_TESS_GEN_SPACING_EXT           0x8E77
#define GL_TESS_GEN_VERTEX_ORDER_EXT      0x8E78
#define GL_TESS_GEN_POINT_MODE_EXT        0x8E79
#define GL_ISOLINES_EXT                   0x8E7A
#define GL_QUADS_EXT                      0x0007
#define GL_FRACTIONAL_ODD_EXT             0x8E7B
#define GL_FRACTIONAL_EVEN_EXT            0x8E7C
#define GL_MAX_PATCH_VERTICES_EXT         0x8E7D
#define GL_MAX_TESS_GEN_LEVEL_EXT         0x8E7E
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS_EXT 0x8E7F
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS_EXT 0x8E80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS_EXT 0x8E81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS_EXT 0x8E82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS_EXT 0x8E83
#define GL_MAX_TESS_PATCH_COMPONENTS_EXT  0x8E84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS_EXT 0x8E85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS_EXT 0x8E86
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS_EXT 0x8E89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS_EXT 0x8E8A
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS_EXT 0x886C
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS_EXT 0x886D
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS_EXT 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS_EXT 0x8E1F
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS_EXT 0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS_EXT 0x92CE
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS_EXT 0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS_EXT 0x92D4
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS_EXT 0x90CB
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS_EXT 0x90CC
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS_EXT 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS_EXT 0x90D9
#define GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED 0x8221
#define GL_IS_PER_PATCH_EXT               0x92E7
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER_EXT 0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER_EXT 0x9308
#define GL_TESS_CONTROL_SHADER_EXT        0x8E88
#define GL_TESS_EVALUATION_SHADER_EXT     0x8E87
#define GL_TESS_CONTROL_SHADER_BIT_EXT    0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT_EXT 0x00000010

#define GL_RED_EXT                        0x1903
#define GL_RG_EXT                         0x8227
#define GL_R8_EXT                         0x8229
#define GL_RG8_EXT                        0x822B

#define GL_HALF_FLOAT_OES                 0x8D61

#ifdef __cplusplus
}
#endif


#endif  /* __PRG_GL_COMPAT_H__ */
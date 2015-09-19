#include "gl-compat.h"

static PgdGraphicsOpenGlCompat_e pgdGraphicsGlCompat = PgdGraphicsOpenGlCompat_Unknown;

PgdGraphicsOpenGlCompat_e pgdGlGetCompatVersion() {
    return pgdGraphicsGlCompat;
}

/**
 * OpenGL関数ポインタの初期化を行う
 */
void pgdGlCompatInitialize(pgdGetGlProcAddress_ptr getGlProcAddress, void *userData) {
    if (pgdGraphicsGlCompat) {
        return;
    }

    int loadRequestNum = 0;
    int loadSuccessNum = 0;
#define     PGD_LAOD_OPENGL_PROC(ret, name) name = (getGlProcAddress)(userData, #name);  if(name){ ++loadSuccessNum; } ++loadRequestNum

/******************************************************************************/
/*                         OpenGL ES 2.0 compat                               */
/******************************************************************************/
    PGD_LAOD_OPENGL_PROC(void, glActiveTexture);

    PGD_LAOD_OPENGL_PROC(void, glAttachShader);

    PGD_LAOD_OPENGL_PROC(void, glBindAttribLocation);

    PGD_LAOD_OPENGL_PROC(void, glBindBuffer);

    PGD_LAOD_OPENGL_PROC(void, glBindFramebuffer);

    PGD_LAOD_OPENGL_PROC(void, glBindRenderbuffer);

    PGD_LAOD_OPENGL_PROC(void, glBindTexture);

    PGD_LAOD_OPENGL_PROC(void, glBlendColor);

    PGD_LAOD_OPENGL_PROC(void, glBlendEquation);

    PGD_LAOD_OPENGL_PROC(void, glBlendEquationSeparate);

    PGD_LAOD_OPENGL_PROC(void, glBlendFunc);

    PGD_LAOD_OPENGL_PROC(void, glBlendFuncSeparate);

    PGD_LAOD_OPENGL_PROC(void, glBufferData);

    PGD_LAOD_OPENGL_PROC(void, glBufferSubData);

    PGD_LAOD_OPENGL_PROC(GLenum, glCheckFramebufferStatus);

    PGD_LAOD_OPENGL_PROC(void, glClear);

    PGD_LAOD_OPENGL_PROC(void, glClearColor);

    PGD_LAOD_OPENGL_PROC(void, glClearDepthf);

    PGD_LAOD_OPENGL_PROC(void, glClearStencil);

    PGD_LAOD_OPENGL_PROC(void, glColorMask);

    PGD_LAOD_OPENGL_PROC(void, glCompileShader);

    PGD_LAOD_OPENGL_PROC(void, glCompressedTexImage2D);

    PGD_LAOD_OPENGL_PROC(void, glCompressedTexSubImage2D);

    PGD_LAOD_OPENGL_PROC(void, glCopyTexImage2D);

    PGD_LAOD_OPENGL_PROC(void, glCopyTexSubImage2D);

    PGD_LAOD_OPENGL_PROC(GLuint, glCreateProgram);

    PGD_LAOD_OPENGL_PROC(GLuint, glCreateShader);

    PGD_LAOD_OPENGL_PROC(void, glCullFace);

    PGD_LAOD_OPENGL_PROC(void, glDeleteBuffers);

    PGD_LAOD_OPENGL_PROC(void, glDeleteFramebuffers);

    PGD_LAOD_OPENGL_PROC(void, glDeleteProgram);

    PGD_LAOD_OPENGL_PROC(void, glDeleteRenderbuffers);

    PGD_LAOD_OPENGL_PROC(void, glDeleteShader);

    PGD_LAOD_OPENGL_PROC(void, glDeleteTextures);

    PGD_LAOD_OPENGL_PROC(void, glDepthFunc);

    PGD_LAOD_OPENGL_PROC(void, glDepthMask);

    PGD_LAOD_OPENGL_PROC(void, glDepthRangef);

    PGD_LAOD_OPENGL_PROC(void, glDetachShader);

    PGD_LAOD_OPENGL_PROC(void, glDisable);

    PGD_LAOD_OPENGL_PROC(void, glDisableVertexAttribArray);

    PGD_LAOD_OPENGL_PROC(void, glDrawArrays);

    PGD_LAOD_OPENGL_PROC(void, glDrawElements);

    PGD_LAOD_OPENGL_PROC(void, glEnable);

    PGD_LAOD_OPENGL_PROC(void, glEnableVertexAttribArray);

    PGD_LAOD_OPENGL_PROC(void, glFinish);

    PGD_LAOD_OPENGL_PROC(void, glFlush);

    PGD_LAOD_OPENGL_PROC(void, glFramebufferRenderbuffer);

    PGD_LAOD_OPENGL_PROC(void, glFramebufferTexture2D);

    PGD_LAOD_OPENGL_PROC(void, glFrontFace);

    PGD_LAOD_OPENGL_PROC(void, glGenBuffers);

    PGD_LAOD_OPENGL_PROC(void, glGenerateMipmap);

    PGD_LAOD_OPENGL_PROC(void, glGenFramebuffers);

    PGD_LAOD_OPENGL_PROC(void, glGenRenderbuffers);

    PGD_LAOD_OPENGL_PROC(void, glGenTextures);

    PGD_LAOD_OPENGL_PROC(void, glGetActiveAttrib);

    PGD_LAOD_OPENGL_PROC(void, glGetActiveUniform);

    PGD_LAOD_OPENGL_PROC(void, glGetAttachedShaders);

    PGD_LAOD_OPENGL_PROC(GLint, glGetAttribLocation);

    PGD_LAOD_OPENGL_PROC(void, glGetBooleanv);

    PGD_LAOD_OPENGL_PROC(void, glGetBufferParameteriv);

    PGD_LAOD_OPENGL_PROC(GLenum, glGetError);

    PGD_LAOD_OPENGL_PROC(void, glGetFloatv);

    PGD_LAOD_OPENGL_PROC(void, glGetFramebufferAttachmentParameteriv);

    PGD_LAOD_OPENGL_PROC(void, glGetIntegerv);

    PGD_LAOD_OPENGL_PROC(void, glGetProgramiv);

    PGD_LAOD_OPENGL_PROC(void, glGetProgramInfoLog);

    PGD_LAOD_OPENGL_PROC(void, glGetRenderbufferParameteriv);

    PGD_LAOD_OPENGL_PROC(void, glGetShaderiv);

    PGD_LAOD_OPENGL_PROC(void, glGetShaderInfoLog);

    PGD_LAOD_OPENGL_PROC(void, glGetShaderPrecisionFormat);

    PGD_LAOD_OPENGL_PROC(void, glGetShaderSource);

    PGD_LAOD_OPENGL_PROC(const GLubyte *, glGetString);

    PGD_LAOD_OPENGL_PROC(void, glGetTexParameterfv);

    PGD_LAOD_OPENGL_PROC(void, glGetTexParameteriv);

    PGD_LAOD_OPENGL_PROC(void, glGetUniformfv);

    PGD_LAOD_OPENGL_PROC(void, glGetUniformiv);

    PGD_LAOD_OPENGL_PROC(GLint, glGetUniformLocation);

    PGD_LAOD_OPENGL_PROC(void, glGetVertexAttribfv);

    PGD_LAOD_OPENGL_PROC(void, glGetVertexAttribiv);

    PGD_LAOD_OPENGL_PROC(void, glGetVertexAttribPointerv);

    PGD_LAOD_OPENGL_PROC(void, glHint);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsBuffer);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsEnabled);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsFramebuffer);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsProgram);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsRenderbuffer);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsShader);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsTexture);

    PGD_LAOD_OPENGL_PROC(void, glLineWidth);

    PGD_LAOD_OPENGL_PROC(void, glLinkProgram);

    PGD_LAOD_OPENGL_PROC(void, glPixelStorei);

    PGD_LAOD_OPENGL_PROC(void, glPolygonOffset);

    PGD_LAOD_OPENGL_PROC(void, glReadPixels);

    PGD_LAOD_OPENGL_PROC(void, glReleaseShaderCompiler);

    PGD_LAOD_OPENGL_PROC(void, glRenderbufferStorage);

    PGD_LAOD_OPENGL_PROC(void, glSampleCoverage);

    PGD_LAOD_OPENGL_PROC(void, glScissor);

    PGD_LAOD_OPENGL_PROC(void, glShaderBinary);

    PGD_LAOD_OPENGL_PROC(void, glShaderSource);

    PGD_LAOD_OPENGL_PROC(void, glStencilFunc);

    PGD_LAOD_OPENGL_PROC(void, glStencilFuncSeparate);

    PGD_LAOD_OPENGL_PROC(void, glStencilMask);

    PGD_LAOD_OPENGL_PROC(void, glStencilMaskSeparate);

    PGD_LAOD_OPENGL_PROC(void, glStencilOp);

    PGD_LAOD_OPENGL_PROC(void, glStencilOpSeparate);

    PGD_LAOD_OPENGL_PROC(void, glTexImage2D);

    PGD_LAOD_OPENGL_PROC(void, glTexParameterf);

    PGD_LAOD_OPENGL_PROC(void, glTexParameterfv);

    PGD_LAOD_OPENGL_PROC(void, glTexParameteri);

    PGD_LAOD_OPENGL_PROC(void, glTexParameteriv);

    PGD_LAOD_OPENGL_PROC(void, glTexSubImage2D);

    PGD_LAOD_OPENGL_PROC(void, glUniform1f);

    PGD_LAOD_OPENGL_PROC(void, glUniform1fv);

    PGD_LAOD_OPENGL_PROC(void, glUniform1i);

    PGD_LAOD_OPENGL_PROC(void, glUniform1iv);

    PGD_LAOD_OPENGL_PROC(void, glUniform2f);

    PGD_LAOD_OPENGL_PROC(void, glUniform2fv);

    PGD_LAOD_OPENGL_PROC(void, glUniform2i);

    PGD_LAOD_OPENGL_PROC(void, glUniform2iv);

    PGD_LAOD_OPENGL_PROC(void, glUniform3f);

    PGD_LAOD_OPENGL_PROC(void, glUniform3fv);

    PGD_LAOD_OPENGL_PROC(void, glUniform3i);

    PGD_LAOD_OPENGL_PROC(void, glUniform3iv);

    PGD_LAOD_OPENGL_PROC(void, glUniform4f);

    PGD_LAOD_OPENGL_PROC(void, glUniform4fv);

    PGD_LAOD_OPENGL_PROC(void, glUniform4i);

    PGD_LAOD_OPENGL_PROC(void, glUniform4iv);

    PGD_LAOD_OPENGL_PROC(void, glUniformMatrix2fv);

    PGD_LAOD_OPENGL_PROC(void, glUniformMatrix3fv);

    PGD_LAOD_OPENGL_PROC(void, glUniformMatrix4fv);

    PGD_LAOD_OPENGL_PROC(void, glUseProgram);

    PGD_LAOD_OPENGL_PROC(void, glValidateProgram);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttrib1f);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttrib1fv);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttrib2f);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttrib2fv);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttrib3f);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttrib3fv);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttrib4f);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttrib4fv);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttribPointer);

    PGD_LAOD_OPENGL_PROC(void, glViewport);

    if (loadRequestNum == loadSuccessNum) {
        pgdGraphicsGlCompat = PgdGraphicsOpenGlCompat_ES20;
    }

/******************************************************************************/
/*                         OpenGL ES 3.0 compat                               */
/******************************************************************************/

    PGD_LAOD_OPENGL_PROC(void, glReadBuffer);

    PGD_LAOD_OPENGL_PROC(void, glDrawRangeElements);

    PGD_LAOD_OPENGL_PROC(void, glTexImage3D);

    PGD_LAOD_OPENGL_PROC(void, glTexSubImage3D);

    PGD_LAOD_OPENGL_PROC(void, glCopyTexSubImage3D);

    PGD_LAOD_OPENGL_PROC(void, glCompressedTexImage3D);

    PGD_LAOD_OPENGL_PROC(void, glCompressedTexSubImage3D);

    PGD_LAOD_OPENGL_PROC(void, glGenQueries);

    PGD_LAOD_OPENGL_PROC(void, glDeleteQueries);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsQuery);

    PGD_LAOD_OPENGL_PROC(void, glBeginQuery);

    PGD_LAOD_OPENGL_PROC(void, glEndQuery);

    PGD_LAOD_OPENGL_PROC(void, glGetQueryiv);

    PGD_LAOD_OPENGL_PROC(void, glGetQueryObjectuiv);

    PGD_LAOD_OPENGL_PROC(GLboolean, glUnmapBuffer);

    PGD_LAOD_OPENGL_PROC(void, glGetBufferPointerv);

    PGD_LAOD_OPENGL_PROC(void, glDrawBuffers);

    PGD_LAOD_OPENGL_PROC(void, glUniformMatrix2x3fv);

    PGD_LAOD_OPENGL_PROC(void, glUniformMatrix3x2fv);

    PGD_LAOD_OPENGL_PROC(void, glUniformMatrix2x4fv);

    PGD_LAOD_OPENGL_PROC(void, glUniformMatrix4x2fv);

    PGD_LAOD_OPENGL_PROC(void, glUniformMatrix3x4fv);

    PGD_LAOD_OPENGL_PROC(void, glUniformMatrix4x3fv);

    PGD_LAOD_OPENGL_PROC(void, glBlitFramebuffer);

    PGD_LAOD_OPENGL_PROC(void, glRenderbufferStorageMultisample);

    PGD_LAOD_OPENGL_PROC(void, glFramebufferTextureLayer);

    PGD_LAOD_OPENGL_PROC(void *, glMapBufferRange);

    PGD_LAOD_OPENGL_PROC(void, glFlushMappedBufferRange);

    PGD_LAOD_OPENGL_PROC(void, glBindVertexArray);

    PGD_LAOD_OPENGL_PROC(void, glDeleteVertexArrays);

    PGD_LAOD_OPENGL_PROC(void, glGenVertexArrays);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsVertexArray);

    PGD_LAOD_OPENGL_PROC(void, glGetIntegeri_v);

    PGD_LAOD_OPENGL_PROC(void, glBeginTransformFeedback);

    PGD_LAOD_OPENGL_PROC(void, glEndTransformFeedback);

    PGD_LAOD_OPENGL_PROC(void, glBindBufferRange);

    PGD_LAOD_OPENGL_PROC(void, glBindBufferBase);

    PGD_LAOD_OPENGL_PROC(void, glTransformFeedbackVaryings);

    PGD_LAOD_OPENGL_PROC(void, glGetTransformFeedbackVarying);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttribIPointer);

    PGD_LAOD_OPENGL_PROC(void, glGetVertexAttribIiv);

    PGD_LAOD_OPENGL_PROC(void, glGetVertexAttribIuiv);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttribI4i);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttribI4ui);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttribI4iv);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttribI4uiv);

    PGD_LAOD_OPENGL_PROC(void, glGetUniformuiv);

    PGD_LAOD_OPENGL_PROC(GLint, glGetFragDataLocation);

    PGD_LAOD_OPENGL_PROC(void, glUniform1ui);

    PGD_LAOD_OPENGL_PROC(void, glUniform2ui);

    PGD_LAOD_OPENGL_PROC(void, glUniform3ui);

    PGD_LAOD_OPENGL_PROC(void, glUniform4ui);

    PGD_LAOD_OPENGL_PROC(void, glUniform1uiv);

    PGD_LAOD_OPENGL_PROC(void, glUniform2uiv);

    PGD_LAOD_OPENGL_PROC(void, glUniform3uiv);

    PGD_LAOD_OPENGL_PROC(void, glUniform4uiv);

    PGD_LAOD_OPENGL_PROC(void, glClearBufferiv);

    PGD_LAOD_OPENGL_PROC(void, glClearBufferuiv);

    PGD_LAOD_OPENGL_PROC(void, glClearBufferfv);

    PGD_LAOD_OPENGL_PROC(void, glClearBufferfi);

    PGD_LAOD_OPENGL_PROC(const GLubyte *, glGetStringi);

    PGD_LAOD_OPENGL_PROC(void, glCopyBufferSubData);

    PGD_LAOD_OPENGL_PROC(void, glGetUniformIndices);

    PGD_LAOD_OPENGL_PROC(void, glGetActiveUniformsiv);

    PGD_LAOD_OPENGL_PROC(GLuint, glGetUniformBlockIndex);

    PGD_LAOD_OPENGL_PROC(void, glGetActiveUniformBlockiv);

    PGD_LAOD_OPENGL_PROC(void, glGetActiveUniformBlockName);

    PGD_LAOD_OPENGL_PROC(void, glUniformBlockBinding);

    PGD_LAOD_OPENGL_PROC(void, glDrawArraysInstanced);

    PGD_LAOD_OPENGL_PROC(void, glDrawElementsInstanced);

    PGD_LAOD_OPENGL_PROC(GLsync, glFenceSync);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsSync);

    PGD_LAOD_OPENGL_PROC(void, glDeleteSync);

    PGD_LAOD_OPENGL_PROC(GLenum, glClientWaitSync);

    PGD_LAOD_OPENGL_PROC(void, glWaitSync);

    PGD_LAOD_OPENGL_PROC(void, glGetInteger64v);

    PGD_LAOD_OPENGL_PROC(void, glGetSynciv);

    PGD_LAOD_OPENGL_PROC(void, glGetInteger64i_v);

    PGD_LAOD_OPENGL_PROC(void, glGetBufferParameteri64v);

    PGD_LAOD_OPENGL_PROC(void, glGenSamplers);

    PGD_LAOD_OPENGL_PROC(void, glDeleteSamplers);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsSampler);

    PGD_LAOD_OPENGL_PROC(void, glBindSampler);

    PGD_LAOD_OPENGL_PROC(void, glSamplerParameteri);

    PGD_LAOD_OPENGL_PROC(void, glSamplerParameteriv);

    PGD_LAOD_OPENGL_PROC(void, glSamplerParameterf);

    PGD_LAOD_OPENGL_PROC(void, glSamplerParameterfv);

    PGD_LAOD_OPENGL_PROC(void, glGetSamplerParameteriv);

    PGD_LAOD_OPENGL_PROC(void, glGetSamplerParameterfv);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttribDivisor);

    PGD_LAOD_OPENGL_PROC(void, glBindTransformFeedback);

    PGD_LAOD_OPENGL_PROC(void, glDeleteTransformFeedbacks);

    PGD_LAOD_OPENGL_PROC(void, glGenTransformFeedbacks);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsTransformFeedback);

    PGD_LAOD_OPENGL_PROC(void, glPauseTransformFeedback);

    PGD_LAOD_OPENGL_PROC(void, glResumeTransformFeedback);

    PGD_LAOD_OPENGL_PROC(void, glGetProgramBinary);

    PGD_LAOD_OPENGL_PROC(void, glProgramBinary);

    PGD_LAOD_OPENGL_PROC(void, glProgramParameteri);

    PGD_LAOD_OPENGL_PROC(void, glInvalidateFramebuffer);

    PGD_LAOD_OPENGL_PROC(void, glInvalidateSubFramebuffer);

    PGD_LAOD_OPENGL_PROC(void, glTexStorage2D);

    PGD_LAOD_OPENGL_PROC(void, glTexStorage3D);

    PGD_LAOD_OPENGL_PROC(void, glGetInternalformativ);

#if defined(__MACH__)
    // Mac OSでは2つの関数を許容する
    loadSuccessNum += 2;
#endif

    if (loadSuccessNum >= loadRequestNum) {
        pgdGraphicsGlCompat = PgdGraphicsOpenGlCompat_ES30;
    }

    PGD_LAOD_OPENGL_PROC(void, glDispatchCompute);

    PGD_LAOD_OPENGL_PROC(void, glDispatchComputeIndirect);

    PGD_LAOD_OPENGL_PROC(void, glDrawArraysIndirect);

    PGD_LAOD_OPENGL_PROC(void, glDrawElementsIndirect);

    PGD_LAOD_OPENGL_PROC(void, glFramebufferParameteri);

    PGD_LAOD_OPENGL_PROC(void, glGetFramebufferParameteriv);

    PGD_LAOD_OPENGL_PROC(void, glGetProgramInterfaceiv);

    PGD_LAOD_OPENGL_PROC(GLuint, glGetProgramResourceIndex);

    PGD_LAOD_OPENGL_PROC(void, glGetProgramResourceName);

    PGD_LAOD_OPENGL_PROC(void, glGetProgramResourceiv);

    PGD_LAOD_OPENGL_PROC(GLint, glGetProgramResourceLocation);

    PGD_LAOD_OPENGL_PROC(void, glUseProgramStages);

    PGD_LAOD_OPENGL_PROC(void, glActiveShaderProgram);

    PGD_LAOD_OPENGL_PROC(GLuint, glCreateShaderProgramv);

    PGD_LAOD_OPENGL_PROC(void, glBindProgramPipeline);

    PGD_LAOD_OPENGL_PROC(void, glDeleteProgramPipelines);

    PGD_LAOD_OPENGL_PROC(void, glGenProgramPipelines);

    PGD_LAOD_OPENGL_PROC(GLboolean, glIsProgramPipeline);

    PGD_LAOD_OPENGL_PROC(void, glGetProgramPipelineiv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform1i);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform2i);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform3i);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform4i);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform1ui);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform2ui);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform3ui);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform4ui);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform1f);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform2f);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform3f);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform4f);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform1iv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform2iv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform3iv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform4iv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform1uiv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform2uiv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform3uiv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform4uiv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform1fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform2fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform3fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniform4fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniformMatrix2fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniformMatrix3fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniformMatrix4fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniformMatrix2x3fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniformMatrix3x2fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniformMatrix2x4fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniformMatrix4x2fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniformMatrix3x4fv);

    PGD_LAOD_OPENGL_PROC(void, glProgramUniformMatrix4x3fv);

    PGD_LAOD_OPENGL_PROC(void, glValidateProgramPipeline);

    PGD_LAOD_OPENGL_PROC(void, glGetProgramPipelineInfoLog);

    PGD_LAOD_OPENGL_PROC(void, glBindImageTexture);

    PGD_LAOD_OPENGL_PROC(void, glGetBooleani_v);

    PGD_LAOD_OPENGL_PROC(void, glMemoryBarrier);

    PGD_LAOD_OPENGL_PROC(void, glMemoryBarrierByRegion);

    PGD_LAOD_OPENGL_PROC(void, glTexStorage2DMultisample);

    PGD_LAOD_OPENGL_PROC(void, glGetMultisamplefv);

    PGD_LAOD_OPENGL_PROC(void, glSampleMaski);

    PGD_LAOD_OPENGL_PROC(void, glGetTexLevelParameteriv);

    PGD_LAOD_OPENGL_PROC(void, glGetTexLevelParameterfv);

    PGD_LAOD_OPENGL_PROC(void, glBindVertexBuffer);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttribFormat);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttribIFormat);

    PGD_LAOD_OPENGL_PROC(void, glVertexAttribBinding);

    PGD_LAOD_OPENGL_PROC(void, glVertexBindingDivisor);

    if (loadSuccessNum >= loadRequestNum) {
        pgdGraphicsGlCompat = PgdGraphicsOpenGlCompat_ES31;
    }

}

#define PGD_OPENGL_FUNCTION_STUB(ret, name) ret (*name)

/******************************************************************************/
/*                         OpenGL ES 2.0 compat                               */
/******************************************************************************/

PGD_OPENGL_FUNCTION_STUB(void, glActiveTexture)(GLenum texture) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glAttachShader)(GLuint program, GLuint shader) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindAttribLocation)(GLuint program, GLuint index, const GLchar *name) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindBuffer)(GLenum target, GLuint buffer) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindFramebuffer)(GLenum target, GLuint framebuffer) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindRenderbuffer)(GLenum target, GLuint renderbuffer) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindTexture)(GLenum target, GLuint texture) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBlendEquation)(GLenum mode) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBlendFunc)(GLenum sfactor, GLenum dfactor) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBufferData)(GLenum target, GLsizeiptr size, const void *data, GLenum usage) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data) = 0;

PGD_OPENGL_FUNCTION_STUB(GLenum, glCheckFramebufferStatus)(GLenum target) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glClear)(GLbitfield mask) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glClearDepthf)(GLfloat d) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glClearStencil)(GLint s) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glCompileShader)(GLuint shader) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) = 0;

PGD_OPENGL_FUNCTION_STUB(GLuint, glCreateProgram)(void) = 0;

PGD_OPENGL_FUNCTION_STUB(GLuint, glCreateShader)(GLenum type) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glCullFace)(GLenum mode) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteBuffers)(GLsizei n, const GLuint *buffers) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteProgram)(GLuint program) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteShader)(GLuint shader) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteTextures)(GLsizei n, const GLuint *textures) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDepthFunc)(GLenum func) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDepthMask)(GLboolean flag) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDepthRangef)(GLfloat n, GLfloat f) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDetachShader)(GLuint program, GLuint shader) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDisable)(GLenum cap) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDisableVertexAttribArray)(GLuint index) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDrawArrays)(GLenum mode, GLint first, GLsizei count) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glEnable)(GLenum cap) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glEnableVertexAttribArray)(GLuint index) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glFinish)(void) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glFlush)(void) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glFrontFace)(GLenum mode) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGenBuffers)(GLsizei n, GLuint *buffers) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGenerateMipmap)(GLenum target) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGenFramebuffers)(GLsizei n, GLuint *framebuffers) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGenTextures)(GLsizei n, GLuint *textures) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders) = 0;

PGD_OPENGL_FUNCTION_STUB(GLint, glGetAttribLocation)(GLuint program, const GLchar *name) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetBooleanv)(GLenum pname, GLboolean *data) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(GLenum, glGetError)(void) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetFloatv)(GLenum pname, GLfloat *data) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetIntegerv)(GLenum pname, GLint *data) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetProgramiv)(GLuint program, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetShaderiv)(GLuint shader, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) = 0;

PGD_OPENGL_FUNCTION_STUB(const GLubyte *, glGetString)(GLenum name) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetUniformfv)(GLuint program, GLint location, GLfloat *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetUniformiv)(GLuint program, GLint location, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(GLint, glGetUniformLocation)(GLuint program, const GLchar *name) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetVertexAttribPointerv)(GLuint index, GLenum pname, void **pointer) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glHint)(GLenum target, GLenum mode) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsBuffer)(GLuint buffer) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsEnabled)(GLenum cap) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsFramebuffer)(GLuint framebuffer) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsProgram)(GLuint program) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsRenderbuffer)(GLuint renderbuffer) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsShader)(GLuint shader) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsTexture)(GLuint texture) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glLineWidth)(GLfloat width) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glLinkProgram)(GLuint program) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glPixelStorei)(GLenum pname, GLint param) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glPolygonOffset)(GLfloat factor, GLfloat units) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glReleaseShaderCompiler)(void) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glSampleCoverage)(GLfloat value, GLboolean invert) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glScissor)(GLint x, GLint y, GLsizei width, GLsizei height) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glShaderBinary)(GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glShaderSource)(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glStencilFunc)(GLenum func, GLint ref, GLuint mask) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glStencilMask)(GLuint mask) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glStencilMaskSeparate)(GLenum face, GLuint mask) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTexParameterf)(GLenum target, GLenum pname, GLfloat param) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTexParameteri)(GLenum target, GLenum pname, GLint param) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTexParameteriv)(GLenum target, GLenum pname, const GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform1f)(GLint location, GLfloat v0) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform1fv)(GLint location, GLsizei count, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform1i)(GLint location, GLint v0) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform1iv)(GLint location, GLsizei count, const GLint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform2f)(GLint location, GLfloat v0, GLfloat v1) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform2fv)(GLint location, GLsizei count, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform2i)(GLint location, GLint v0, GLint v1) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform2iv)(GLint location, GLsizei count, const GLint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform3fv)(GLint location, GLsizei count, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform3iv)(GLint location, GLsizei count, const GLint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform4fv)(GLint location, GLsizei count, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform4iv)(GLint location, GLsizei count, const GLint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUseProgram)(GLuint program) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glValidateProgram)(GLuint program) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttrib1f)(GLuint index, GLfloat x) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttrib1fv)(GLuint index, const GLfloat *v) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttrib2fv)(GLuint index, const GLfloat *v) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttrib3fv)(GLuint index, const GLfloat *v) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttrib4fv)(GLuint index, const GLfloat *v) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glViewport)(GLint x, GLint y, GLsizei width, GLsizei height) = 0;

/******************************************************************************/
/*                         OpenGL ES 3.0 compat                               */
/******************************************************************************/

PGD_OPENGL_FUNCTION_STUB(void, glReadBuffer)(GLenum mode) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGenQueries)(GLsizei n, GLuint *ids) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteQueries)(GLsizei n, const GLuint *ids) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsQuery)(GLuint id) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBeginQuery)(GLenum target, GLuint id) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glEndQuery)(GLenum target) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetQueryiv)(GLenum target, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glUnmapBuffer)(GLenum target) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetBufferPointerv)(GLenum target, GLenum pname, void **params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDrawBuffers)(GLsizei n, const GLenum *bufs) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) = 0;

PGD_OPENGL_FUNCTION_STUB(void *, glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindVertexArray)(GLuint array) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteVertexArrays)(GLsizei n, const GLuint *arrays) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGenVertexArrays)(GLsizei n, GLuint *arrays) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsVertexArray)(GLuint array) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetIntegeri_v)(GLenum target, GLuint index, GLint *data) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBeginTransformFeedback)(GLenum primitiveMode) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glEndTransformFeedback)(void) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindBufferBase)(GLenum target, GLuint index, GLuint buffer) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar *const *varyings, GLenum bufferMode) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttribI4iv)(GLuint index, const GLint *v) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttribI4uiv)(GLuint index, const GLuint *v) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetUniformuiv)(GLuint program, GLint location, GLuint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(GLint, glGetFragDataLocation)(GLuint program, const GLchar *name) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform1ui)(GLint location, GLuint v0) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform2ui)(GLint location, GLuint v0, GLuint v1) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform1uiv)(GLint location, GLsizei count, const GLuint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform2uiv)(GLint location, GLsizei count, const GLuint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform3uiv)(GLint location, GLsizei count, const GLuint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniform4uiv)(GLint location, GLsizei count, const GLuint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) = 0;

PGD_OPENGL_FUNCTION_STUB(const GLubyte *, glGetStringi)(GLenum name, GLuint index) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar *const *uniformNames, GLuint *uniformIndices) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(GLuint, glGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount) = 0;

PGD_OPENGL_FUNCTION_STUB(GLsync, glFenceSync)(GLenum condition, GLbitfield flags) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsSync)(GLsync sync) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteSync)(GLsync sync) = 0;

PGD_OPENGL_FUNCTION_STUB(GLenum, glClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetInteger64v)(GLenum pname, GLint64 *data) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetInteger64i_v)(GLenum target, GLuint index, GLint64 *data) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetBufferParameteri64v)(GLenum target, GLenum pname, GLint64 *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGenSamplers)(GLsizei count, GLuint *samplers) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteSamplers)(GLsizei count, const GLuint *samplers) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsSampler)(GLuint sampler) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindSampler)(GLuint unit, GLuint sampler) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *param) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *param) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttribDivisor)(GLuint index, GLuint divisor) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindTransformFeedback)(GLenum target, GLuint id) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteTransformFeedbacks)(GLsizei n, const GLuint *ids) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGenTransformFeedbacks)(GLsizei n, GLuint *ids) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsTransformFeedback)(GLuint id) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glPauseTransformFeedback)(void) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glResumeTransformFeedback)(void) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramBinary)(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramParameteri)(GLuint program, GLenum pname, GLint value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glInvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glInvalidateSubFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTexStorage2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTexStorage3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetInternalformativ)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params) = 0;

/******************************************************************************/
/*                         OpenGL ES 3.1 compat                               */
/******************************************************************************/

PGD_OPENGL_FUNCTION_STUB(void, glDispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDispatchComputeIndirect)(GLintptr indirect) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDrawArraysIndirect)(GLenum mode, const void *indirect) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDrawElementsIndirect)(GLenum mode, GLenum type, const void *indirect) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glFramebufferParameteri)(GLenum target, GLenum pname, GLint param) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetFramebufferParameteriv)(GLenum target, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(GLuint, glGetProgramResourceIndex)(GLuint program, GLenum programInterface, const GLchar *name) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(GLint, glGetProgramResourceLocation)(GLuint program, GLenum programInterface, const GLchar *name) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glActiveShaderProgram)(GLuint pipeline, GLuint program) = 0;

PGD_OPENGL_FUNCTION_STUB(GLuint, glCreateShaderProgramv)(GLenum type, GLsizei count, const GLchar *const *strings) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindProgramPipeline)(GLuint pipeline) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glDeleteProgramPipelines)(GLsizei n, const GLuint *pipelines) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGenProgramPipelines)(GLsizei n, GLuint *pipelines) = 0;

PGD_OPENGL_FUNCTION_STUB(GLboolean, glIsProgramPipeline)(GLuint pipeline) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform1i)(GLuint program, GLint location, GLint v0) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform1ui)(GLuint program, GLint location, GLuint v0) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform1f)(GLuint program, GLint location, GLfloat v0) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glValidateProgramPipeline)(GLuint pipeline) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetBooleani_v)(GLenum target, GLuint index, GLboolean *data) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glMemoryBarrier)(GLbitfield barriers) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glMemoryBarrierByRegion)(GLbitfield barriers) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glTexStorage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glSampleMaski)(GLuint maskNumber, GLbitfield mask) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glBindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttribFormat)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttribIFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexAttribBinding)(GLuint attribindex, GLuint bindingindex) = 0;

PGD_OPENGL_FUNCTION_STUB(void, glVertexBindingDivisor)(GLuint bindingindex, GLuint divisor) = 0;

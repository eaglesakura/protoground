#include "GLShaderProgram.h"
#include "es/internal/protoground-internal.hpp"
#include <vector>

namespace es {
namespace gl {


GLShaderProgram::GLShaderProgram(const GLuint program) {
    this->program = program;
    assert(program);
    assert_gl();
}

GLShaderProgram::~GLShaderProgram() {
    dispose();
}

GLGPUCapacity::GLSLVersion_e GLShaderProgram::getGLSLVersion() const {
    return version;
}

GLuint GLShaderProgram::getProgramHandle() const {
    return program;
}

GLint GLShaderProgram::getAttribLocation(const char *name) const {
    if (!program) {
        return -1;
    }
    return glGetAttribLocation(program, name);
}

GLint GLShaderProgram::getUniformLocation(const char *name) const {
    if (!program) {
        return -1;
    }
    return glGetUniformLocation(program, name);
}

GLint GLShaderProgram::getUniformBlockIndex(const char *name) const {
    if(!program) {
        return -1;
    }
    return glGetUniformBlockIndex(program, name);
}

void GLShaderProgram::bind() {
    glUseProgram(program);
    assert_gl();
}


void GLShaderProgram::dispose() {
    if (!program) {
        return;
    }

    glDeleteProgram(program);
    program = 0;
    assert_gl();
}


namespace {

/**
 * シェーダーの読み込みを行う。
 */
static GLint compileShader(const char *shader_source, const GLenum GL_XXXX_SHADER) {
    const GLint shader = glCreateShader(GL_XXXX_SHADER);
    assert_gl();

    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    // コンパイルエラーをチェックする
    {
        GLint compileSuccess = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
        if (compileSuccess == GL_FALSE) {
            // エラーが発生した
            GLint infoLen = 0;
            // エラーメッセージを取得
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                GLchar *msg = (GLchar *) calloc(infoLen, sizeof(GLchar));
                glGetShaderInfoLog(shader, infoLen, NULL, msg);

                eslog("%s", msg);
                free((void *) msg);
            } else {
                eslog("comple error not info...");
            }
        }

        // コンパイル失敗していたら0を返却する
        if (compileSuccess != GL_TRUE) {
            glDeleteShader(shader);
            return 0;
        }
    }

    return shader;
}

/**
 * 頂点・フラグメントシェーダーをリンクし、ShaderProgramを作成する
 */
static GLuint buildProgram(const char *vertex_shader_source, const char *fragment_shader_source) {
    const GLuint vertex_shader = compileShader(vertex_shader_source, GL_VERTEX_SHADER);
    if (!vertex_shader) {
        eslog("vertex shader compile error");
        return 0;
    }

    const GLuint fragment_shader = compileShader(fragment_shader_source, GL_FRAGMENT_SHADER);
    if (!fragment_shader) {
        eslog("fragment shader compile error");
        return 0;
    }

    const GLuint program = glCreateProgram();
    assert_gl();
    assert(program != 0);

    glAttachShader(program, vertex_shader); // バーテックスシェーダーとプログラムを関連付ける
    glAttachShader(program, fragment_shader); // フラグメントシェーダーとプログラムを関連付ける
    assert_gl();

    // コンパイルを行う
    glLinkProgram(program);

    // リンクエラーをチェックする
    {
        GLint linkSuccess = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
        if (linkSuccess == GL_FALSE) {
            // エラーが発生した
            GLint infoLen = 0;
            // エラーメッセージを取得
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                GLchar *msg = (GLchar *) calloc(infoLen, sizeof(GLchar));
                glGetProgramInfoLog(program, infoLen, NULL, msg);
                eslog("%s", msg);
                free((void *) msg);
            } else {
                eslog("Link Failed(Not Information)");
            }
        }

        // コンパイル失敗していたら0を返却する
        if (linkSuccess != GL_TRUE) {
            eslog("link error ...");
            if (vertex_shader) {
                glDeleteShader(vertex_shader);
            }
            if (fragment_shader) {
                glDeleteShader(fragment_shader);
            }
            glDeleteProgram(program);
            return 0;
        }
    }

    // リンク済みのため、個々のシェーダーオブジェクトの解放フラグを立てる
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    assert_gl();

    // リンク済みのプログラムを返す
    return program;
}

}


::std::shared_ptr<GLShaderProgram> GLShaderProgram::build(const GLGPUCapacity &caps, const std::string &vertex_shader, const std::string &fragment_shader, const GLGPUCapacity::GLSLVersion_e version) {
    std::vector<std::string> shaderText = {
            vertex_shader,
            fragment_shader
    };

    for (std::string &source : shaderText) {
        if (source.find("#version") == std::string::npos) {
            std::string version_string;

            if (version == GLGPUCapacity::GLSLVersion_100) {
                version_string = std::string("#version 100\n/* eglibrary OpenGL ES 2.0 compat */\n");
            } else if (caps.isES()) {
                if (version == GLGPUCapacity::GLSLVersion_310es) {
                    version_string = std::string("#version 310 es\n/* eglibrary OpenGL ES 3.1 */\n");
                } else {
                    version_string = std::string("#version 300 es\n/* eglibrary OpenGL ES 3.0 */\n");
                }
            } else {
                version_string = std::string("#version 410\n/* eglibrary OpenGL 4.1 */\n");
            }
            source = version_string + source;
        }
    }


    GLuint program = buildProgram(shaderText[0].c_str(), shaderText[1].c_str());
    if (!program) {
        eslog("error vert shader\n%s", shaderText[0].c_str());
        eslog("error frag shader\n%s", shaderText[1].c_str());

        return std::shared_ptr<GLShaderProgram>();
    }

    std::shared_ptr<GLShaderProgram> result(new GLShaderProgram(program));
    result->version = version;
    return result;
}

}
}

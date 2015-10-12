#pragma once

#include <es/memory/query_ptr.hpp>
#include "es/protoground.hpp"
#include "es/graphics/engine/IGPUCapacity.hpp"
#include "OpenGLEngine.h"

namespace es {
namespace gl {

class OpenGLEngine;

class GLGPUCapacity : public virtual IGPUCapacity {
public:
    /**
     * OpenGL Shader Languageのバージョンを示す
     * GL互換バージョン : XXnnn
     * ES互換バージョン : nnXXn
     * ES対応ならば1   : nnnnX
     */
    enum GLSLVersion_e {
        GLSLVersion_100 = 30201,
        GLSLVersion_300es = 43301,
        GLSLVersion_310es = 45311,
        GLSLVersion_400 = 40300,
        GLSLVersion_410 = 41300,
    };


    /**
     * OpenGLのバージョンを示す
     * GL互換バージョン : XX
     * ES互換バージョン :   XX
     * GL(1) or ES(0) :     X
     */
    enum GLVersion_e {
        GLVersion_ES20 = 30200,
        GLVersion_ES30 = 40300,
        GLVersion_ES31 = 45310,
        GLVersion_41 = 41301,
        GLVersion_Unknown = 0,
    };

    typedef query_ptr<GLGPUCapacity, InterfaceId_OpenGL_GPUCapacity> query;

    virtual const string getVendor() const override;

    virtual const bool isSupport(const GPUExtension_e extension) const override;

    virtual const GPUFamily_e getGPUFamily() const override;

    virtual QueryResult_e queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const override;

    /**
     * GLSLのバージョン名を取得する
     */
    const string &getGlslVersion() const;

    /**
     * OpenGLのバージョンを取得する
     */
    const GLVersion_e getGLVersion() const;

    /**
     * 頂点属性の最大数を取得する。
     *
     * OpenGL以外のレンダラの場合は常に0
     */
    uint32_t getMaxVertexAttributes() const;

    /**
     * テクスチャユニットの最大数を取得する
     * この枚数を超えるテクスチャ処理は行えない。
     */
    uint32_t getMaxTextureUnits() const;

    /**
     * テクスチャの一辺のサイズの最大値
     */
    uint32_t getMaxTextureSize() const;

    /**
     * 頂点シェーダのUniformベクトル最大数を取得する
     */
    uint32_t getMaxUniformVectorsVs() const;

    /**
     * フラグメントシェーダでのUniformベクトル最大数を取得する
     */
    uint32_t getMaxUniformVectorsFs() const;

    /**
     * ライン描画の最大の太さを取得する
     */
    uint32_t getMaxLineWidth() const;

    /**
     * OpenGL ESであればtrue
     */
    const bool isES() const;

    virtual ~GLGPUCapacity() = default;

private:
    friend class OpenGLEngine;

    GLGPUCapacity();


    class Impl;

    sp<Impl> impl;
};

}
}

﻿#pragma once

#include "es/protoground.hpp"
#include "es/graphics/resource/IMesh.hpp"
#include "es/OpenGL.hpp"
#include <vector>
#include <es/graphics/gl/engine/GLGPUCapacity.h>
#include "es/graphics/model/VertexAttribute.h"

namespace es {
namespace gl {

/**
 * OpenGL ES 3.0互換のメッシュを管理する
 *
 * 1. オブジェクト生成
 * 2. allocでメモリを確保
 * 3. lock/unlockでオブジェクトの内容を転送
 * 4. bindArrayObjectでThreadContextに対応する
 * 5. addAttributeで属性情報を構築する
 */
class GLMesh : public virtual IMesh {
public:
    typedef query_ptr<GLMesh, InterfaceId_OpenGL_Mesh> query;

    struct Attribute;

    GLMesh(std::shared_ptr<IDevice> device);

    virtual ~GLMesh();

    /**
     * バッファオブジェクトをThreadContextに対してバインドする
     *
     * レンダリングThreadを移動させる際に、利用Threadでバインドする必要がある。
     * VAO非対応の場合は何もしない。
     */
    virtual void bindArrayObject();

    /**
     * バッファオブジェクトをThreadContextに対して解放する。
     *
     * レンダリングThreadを移動させる際に解放する必要がある。
     * VAO非対応の場合は何もしない。
     */
    virtual void unbindArrayObject();

    /**
     * Contextに関連付ける
     *
     * レンダリング前にかならず呼び出すこと。
     */
    virtual void bind();

    /**
     * レンダリングを行う
     * @param mode レンダリングモードを指定する GL_TRIANGLES GL_TRIANGLE_FAN...
     */
    virtual void rendering(const GLenum mode);

    /**
     * 指定した範囲のレンダリングを行う
     * @param mode レンダリングモードを指定する GL_TRIANGLES GL_TRIANGLE_FAN...
     * @param offsetIndices indices[offsetIndices]からレンダリングを開始する
     * @param numIndices 指定した数のインデックスバッファを描画する
     */
    virtual void rendering(const GLenum mode, const GLsizei offsetIndices, const GLsizei numIndices);

    /**
     * インデックスバッファを利用せずに描画する
     *
     * @param mode レンダリングモードを指定する GL_TRIANGLES GL_TRIANGLE_FAN...
     * @param offsetVertices vertices[offsetVertices]からレンダリングを開始する
     * @param numVertices 指定した数の頂点を描画する
     */
    virtual void renderingArray(const GLenum mode, const GLsizei offsetVertices, const GLsizei numVertices);


    /**
     * レンダリング用の属性情報を追加する
     */
    virtual void addAttribute(const Attribute &attr);

    virtual void *lock(std::shared_ptr<IDevice> device, const LockOption &option) override;

    virtual void unlock(std::shared_ptr<IDevice> device, const void *ptr) override;

    virtual void alloc(std::shared_ptr<IDevice> device, const AllocOption &option) override;

    virtual unsigned getVertexCount() const override;

    virtual unsigned getIndexCount() const override;

    virtual void dispose() override;

    virtual QueryResult_e queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const override;

    /**
     * 属性情報を生成する
     */
    static Attribute makeAttribute(const VertexAttribute &vertexAttribute, const VertexAttribute::Complex &complex, const GLint location);

    /**
     * 設定する属性情報
     */
    struct Attribute {
        /**
         * 属性数(Vec3 = 3, Vec2 = 2)
         */
        uint8_t size;

        /**
         * 頂点の隙間量
         * sizeof(VertexStruct)
         */
        uint8_t strideBytes;

        /**
         * GL_UNSIGNED_BYTE -> GL_FLOAT変換時に0.0〜1.0に丸める場合はGL_TRUE
         */
        bool normalize;

        /**
         * 頂点構造体から実際の属性までのオフセット値 {vec3, vec2}で後半にアクセスする場合はsizeof(vec3)を指定する
         */
        GLint offsetHeader;

        /**
         * attribute loc
         */
        int8_t location;

        /**
         * 属性タイプ(float = GL_FLOAT, byte = GL_UNSIGNED_BYTE）
         */
        GLenum type;

        bool valid() const;
    };

private:

    struct Handle {
        /**
         * VRAMハンドル
         */
        GLuint handle = 0;

        /**
         * データ数
         */
		unsigned num = 0;

        /**
         * 1データのバイト数
         */
		unsigned onceBytes = 0;

        /**
         * ロック済みポインタ
         */
        void *locked = nullptr;
    } vertices, indices;

    bool supportArrayObject = false;

    /**
     * サポートしている場合はVAOを利用する。
     *
     * しない場合は別な方法を利用する。
     */
    GLuint arrayObject = 0;

    /**
     * 利用する属性一覧
     */
    std::vector<Attribute> attributes;
};

}
}



#pragma once

#include "es/protoground.hpp"
#include <es/graphics/PixelFormat.hpp>
#include "es/OpenGL.hpp"
#include <es/graphics/engine/IGraphicsResource.h>
#include <es/graphics/resource/ITexture.hpp>
#include <es/graphics/gl/engine/context/GLTextureState.h>
#include "es/graphics/gl/engine/GLGPUCapacity.h"


namespace es {
namespace gl {

class GLTexture : public virtual IGraphicsResource, public virtual ITexture {
public:
    typedef query_ptr<GLTexture, InterfaceId_OpenGL_Texture> query;
    typedef std::shared_ptr<GLTexture> shared;

    GLTexture();

    /**
     * テクスチャ幅を取得する。
     */
    virtual uint getWidth() const override;

    /**
     * テクスチャの高さを取得する
     */
    virtual uint getHeight() const override;

    /**
     * 画像としての幅と高さを指定する。
     *
     * 例えばPOT変換を行った場合、実際に使用する領域(NPOT)とテクスチャの領域(POT)が異なる場合がある。
     * その場合、画像サイズとしてデータを残しておくことでSpriteを正常に描画する。
     * この設定はSpriteとして使用する際に重要となるが、実際のテクスチャサイズを超えて生成することは出来ない。
     */
    void setImageSize(uint x, uint y);

    /**
     * テクスチャのアスペクト比を取得する
     */
    float getAspect() const;

    /**
     * NPOTテクスチャの場合trueを返す。
     * GPUに渡すステータスをチェックするため、getTextureWidth()とgetTextureHeight()をチェックする
     */
    bool isPowerOfTwoTexture();

    /**
     * ラップモードを指定する
     */
    void setWrapMode(const GLenum s, const GLenum t);

    /**
     * フィルタモードを指定する
     */
    void setFilter(const GLenum min, const GLenum mag);

    /**
     * mipmapを自動生成する
     */
    virtual void genMipmaps();

    /**
     * glBindTexgture(target, xxx)の内容を変更する
     */
    void setBindTarget(const GLenum target);

    GLenum getBindTarget() const;

    /**
     * テクスチャ用メモリを確保する
     */
    void allocPixelMemory(const PixelFormat_e pixelFormat, const int miplevel, const int width, const int height);

    /**
     * 外部要因でallocを行った（拡張機能等）場合に呼び出す
     */
    void onAllocated();


    bool isAllocated() const;

    /**
     * 番号を自動的に付与してバインドする
     *
     * @return バインドしたテクスチャユニット番号 0,1,2...
     */
    uint bind(std::shared_ptr<GLTextureState> state);

    /**
     * テクスチャをindex番のユニットに関連付ける
     */
    void bind(const uint index, std::shared_ptr<GLTextureState> state);

    /**
     * テクスチャを明示的にバインド解除する
     *
     * 複数のテクスチャユニットにバインドされている場合、全て解除される。
     */
    void unbind(std::shared_ptr<GLTextureState> state);

    /**
     * テクスチャ操作ハンドルを取得する
     */
    GLuint getTextureHandle() const;

    /**
     * 管理している資源を開放する
     */
    virtual void dispose() override;

    virtual QueryResult_e queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const override;


    virtual void getImageArea(RectI16 *existArea) const override;


    virtual ITexture::Type_e getType() const override;

    virtual ~GLTexture();

protected:
    struct {
        /**
         * テクスチャサイズとしての幅
         */
        int16_t tex_width = 0;

        /**
         * テクスチャサイズとしての高さ
         */
        int16_t tex_height = 0;

        /**
         * 画像としての幅
         */
        int16_t img_width = 0;

        /**
         * 画像としての高さ
         */
        int16_t img_height = 0;
    } size;

    /**
     * テクスチャバインドターゲット
     */
    GLenum target = GL_TEXTURE_2D;

    /**
     * 状態管理
     */
    struct {
        /**
         * GL_TEXTURE_MAG_FILTER
         * default = GL_NEAREST
         */
        GLenum minFilter = 0;

        /**
         * GL_TEXTURE_MIN_FILTER
         * default = GL_NEAREST
         */
        GLenum magFilter = 0;

        /**
         * GL_TEXTURE_WRAP_S
         * default = GL_CLAMP_TO_EDGE
         */
        GLenum wrapS = 0;

        /**
         * GL_TEXTURE_WRAP_T
         * default = GL_CLAMP_TO_EDGE
         */
        GLenum wrapT = 0;
    } context;

    /**
     * gen texture
     */
    GLuint handle = 0;


    /**
     * テクスチャ領域を確保済みならtrue
     */
    bool allocated = false;
};

}
}



#pragma once

#include <es/graphics/gl/engine/GLGPUCapacity.h>
#include "es/protoground.hpp"
#include "es/OpenGL.hpp"
#include <vector>

namespace es {

namespace gl_util {

/**
 * テクスチャユニットの定数をテクスチャ番号に変換する
 */
inline uint unitToTextureIndex(const GLenum textureUnit) {
    assert(textureUnit >= GL_TEXTURE0);

    return textureUnit - GL_TEXTURE0;
}

/**
 * テクスチャ番号をテクスチャユニット定数に変換する
 */
inline GLenum indexToTextureUnit(const uint index) {
    return index + GL_TEXTURE0;
}

}

namespace gl {

class GLDevice;

class GLTextureState : public virtual Object {
public:
    void syncPlatform();

    /**
     * 指定したテクスチャがバインド済みのユニットのインデックスを取得する
     * バインドされていない場合、0未満を返す。
     */
    int getBindedTextureUnitIndex(const GLenum target, const GLuint texture) const;

    /**
     * テクスチャユニットをActiveにする
     */
    bool activeTexture(const uint index);

    /**
     * 現在アクティブになっているテクスチャの番号を取得する。
     */
    uint getActiveTextureIndex() const;

    /**
     * 現在activeになっているテクスチャユニットに対してバインドを行う。
     */
    bool bindTexture(const GLenum target, const GLuint texture);

    /**
     * まだバインドされているかをチェックする
     */
    bool isBindedTexture(const uint index, const GLenum target, const GLuint texture);

    /**
     * 指定したテクスチャがバインド済みになっているかを調べる
     */
    bool isBindedTexture(const GLenum target, const GLuint texture) const;

    /**
     * バインドされているテクスチャを一括で解放する
     */
    void unbindTextures(const uint num, const GLuint *textures);

    /**
     * 全てのテクスチャを一括で外す
     */
    void unbindTextures();

    /**
     * 引数のテクスチャがどれかにバインドされている場合、バインドを解除する。
     */
    void unbindTexture(const GLuint texture);

    /**
     * 空いているテクスチャユニット番号を取得する
     * この機能は複数枚テクスチャを使用する場合、持ち回りでテクスチャユニットがオーバーライドされる恐れがある。
     * マルチテクスチャの描画ではこのメソッドを使用しないことを推奨する。
     *
     * @param overrride trueの場合、適当なテクスチャユニットをピックアップして返す。
     */
    int getFreeTextureUnitIndex(const bool overrride);

    virtual ~GLTextureState();

private:
    friend class GLDevice;

    GLTextureState(const GLGPUCapacity::query capacity);

    struct context {
        /**
         * バインドされているターゲット
         * GL_TEXTURE_2D等
         */
        GLenum target;

        /**
         * テクスチャハンドル
         * glGenTextures
         */
        GLuint handle;
    };

    /**
     * GLESの定数として、index0〜31までは確保されている。
     * ただし、それがハードウェアとして対応しているかは問わない
     * GL_TEXTURE0〜GL_TEXTURE31に対応
     */
    std::vector<context> contexts;

    /**
     * アクティブ化されているテクスチャユニットを保持する
     */
    uint active;

    /**
     * 空きテクスチャユニットがない場合に上書き取得するテクスチャの番号
     */
    uint overrideTextureUnitIndex = 0;
};

}
}


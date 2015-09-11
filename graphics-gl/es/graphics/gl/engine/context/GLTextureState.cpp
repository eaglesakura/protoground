#include "GLTextureState.h"
#include "es/memory/Buffer.hpp"

namespace es {
namespace gl_util {

}

namespace gl {

GLTextureState::GLTextureState(GLGPUCapacity::query capacity) {
    // reset texture params
    util::valloc(&contexts, capacity->getMaxTextureUnits(), false);
    syncPlatform();
}

GLTextureState::~GLTextureState() {
}


void GLTextureState::syncPlatform() {
    active = GL_TEXTURE0;

    auto itr = contexts.rbegin();
    const auto end = contexts.rend();
    int index = contexts.size() - 1;
    // EGLContext側を解放する
    // テクスチャユニットの大きい方から反対方向に向かって解放して、最後のglActiveTexture分を節約できる
    while (itr != end) {
        glActiveTexture(gl_util::indexToTextureUnit(index));
        glBindTexture(GL_TEXTURE_2D, 0);

        // キャッシュ側をクリアする
        itr->handle = 0;
        itr->target = 0;

        ++itr;
        --index;
    }
}


uint GLTextureState::getActiveTextureIndex() const {
    return gl_util::unitToTextureIndex(active);
}

int GLTextureState::getBindedTextureUnitIndex(const GLenum target, const GLuint texture) const {

    int index = 0;
    for (const context &ctx : contexts) {
        if (ctx.handle == texture && ctx.target == target) {
            return index;
        }
        ++index;
    }
    return -1;
}

bool GLTextureState::isBindedTexture(const uint index, const GLenum target, const GLuint texture) {
    assert(index < contexts.size());
    return contexts[index].handle == texture && contexts[index].target == target;
}

bool GLTextureState::isBindedTexture(const GLenum target, const GLuint texture) const {
    return getBindedTextureUnitIndex(target, texture) >= 0;
}

bool GLTextureState::activeTexture(const uint index) {
    assert(index < contexts.size());
    const uint unit = gl_util::indexToTextureUnit(index);
    // 違うユニットがアクティブ化されていたら、アクティブにし直す
    if (unit != active) {
        active = unit;
        glActiveTexture(unit);
        assert_gl();
        return true;
    }
    return false;
}

bool GLTextureState::bindTexture(const GLenum target, const GLuint texture) {
    const uint index = getActiveTextureIndex();
    assert(index >= 0 && index < contexts.size());

    context &ctx = contexts[index];

    // 違うテクスチャがユニットに設定されていたら、バインドし直す
    if (ctx.handle != texture || ctx.target != target) {
        ctx.handle = texture;
        ctx.target = target;
        glBindTexture(target, texture);
        assert_gl();
        return true;
    }

    return false;
}

void GLTextureState::unbindTextures(const uint num, const GLuint *textures) {
    const uint activeIndex = gl_util::unitToTextureIndex(this->active);

    for (int n = 0; n < num; ++n) {
        const GLuint texture = textures[n];

        int index = 0;
        for (context &ctx : contexts) {
            if (ctx.handle == texture) {
                // テクスチャが一致したからunbind
                activeTexture(index);
                bindTexture(ctx.target, 0);
            }
            ++index;
        }
    }

    activeTexture(activeIndex);
}

void GLTextureState::unbindTextures() {
    const uint active = gl_util::unitToTextureIndex(active);
    int index = 0;
    for (context &ctx : contexts) {
        activeTexture(index);
        bindTexture(GL_TEXTURE_2D, 0);
        ++index;
    }
    activeTexture(active);
}

void GLTextureState::unbindTexture(const GLuint texture) {
    unbindTextures(1, &texture);
}

int GLTextureState::getFreeTextureUnitIndex(const bool overrride) {
    int index = 0;
    for (const context &ctx : contexts) {
        if (ctx.handle == 0) {
            return index;
        }
        ++index;
    }

    //強制的に持ち回りでテクスチャユニットを上書きする
    if (overrride) {
        const int unitIndex = ((++overrideTextureUnitIndex) % contexts.size());
        assert(unitIndex < contexts.size());
        return unitIndex;
    } else {
        // 上書きせずにエラーを返す
        return -1;
    }
}

}
}
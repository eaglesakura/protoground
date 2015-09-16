#include "es/internal/protoground-internal.hpp"
#include "GLDevice.h"
#include <stack>
#include "es/graphics/2d/IDisplayTransfer2D.h"
#include "es/graphics/gl/engine/context/GLTextureState.h"
#include "es/graphics/engine/IRenderingSurface.hpp"

namespace es {
namespace gl {

class GLDevice::Impl : public virtual IDisplayTransfer2D, public virtual IRenderingSurface {
public:
    /**
     * サーフェイスの状態
     */
    std::stack<surface_state> surfaceStates;

    /**
     * 現在のレンダリング状態
     */
    render_state renderState;

    sp<GLTextureState> textureState;

    GLGPUCapacity::query caps;

    virtual vec2 getSpritePositionToDevice(const int x2D, const int y2D) const override {
        const surface_state &cur = surfaceStates.top();
        vec2 result(
                (float) x2D / (float) cur.viewport.width(),
                (float) y2D / (float) cur.viewport.height()
        );
        result.x = (result.x * 2.0f) - 1.0f;
        result.y = -((result.y * 2.0f) - 1.0f);
        return result;
    }

    virtual float getSpriteSurfaceAspect() const override {
        const surface_state &cur = surfaceStates.top();
        return ((float) cur.viewport.width()) / ((float) cur.viewport.height());
    }


    virtual int getWidth() const override {
        const surface_state &cur = surfaceStates.top();
        return cur.viewport.width();
    }

    virtual int getHeight() const override {
        const surface_state &cur = surfaceStates.top();
        return cur.viewport.height();
    }
};

GLDevice::GLDevice(sp<IGPUCapacity> caps) {
    impl.reset(new GLDevice::Impl());
    impl->caps.reset(caps);
    assert(impl->caps);

    impl->surfaceStates.push(surface_state());
    impl->textureState.reset(new GLTextureState(impl->caps));

    syncPlatform();
}

GLDevice::~GLDevice() {
    this->dispose();
    eslog("GLDevice::~GLDevice(%x)", this);
}

void GLDevice::syncPlatform() {
    surface_state &surface = impl->surfaceStates.top();
    render_state &render = impl->renderState;

    memset(&render, 0x00, sizeof(render_state));
    memset(&surface, 0x00, sizeof(surface_state));

    GLint temp;
    // depth check
    if (glIsEnabled(GL_DEPTH_TEST)) {
        render.flags |= render_state::Flag_DepthTest_Enable;
    }
    assert_gl();

    // cull
    if (glIsEnabled(GL_CULL_FACE)) {
        glGetIntegerv(GL_CULL_FACE_MODE, &temp);
        if (temp == GL_FRONT) {
            render.flags |= render_state::Flag_Cull_Front;
        } else {
            render.flags |= render_state::Flag_Cull_Back;
        }
    }
    assert_gl();

    // FrontFace
    glGetIntegerv(GL_FRONT_FACE, &temp);
    if (temp == GL_CW) {
        // 表ポリの反転
        render.flags |= render_state::Flag_FrontFace_CW;
    }
    assert_gl();

    // stencil
    if (glIsEnabled(GL_STENCIL_TEST)) {
        render.flags |= render_state::Flag_StencilTest_Enable;
    }
    assert_gl();

    // viewport
    {
        GLint xywh[4] = {0};
        glGetIntegerv(GL_VIEWPORT, xywh);
        assert_gl();
        surface.viewport.setXYWH((int16_t) xywh[0], (int16_t) xywh[1], (int16_t) xywh[2], (int16_t) xywh[3]);
    }
    // scissor
    {
        GLint xywh[4] = {0};
        glGetIntegerv(GL_SCISSOR_BOX, xywh);
        assert_gl();
        surface.scissor.setXYWH((int16_t) xywh[0], (int16_t) xywh[1], (int16_t) xywh[2], (int16_t) xywh[3]);
    }
    // ブレンドタイプは不明にしておく
    render.blendType = render_state::BlendType_Unknown;

    // テクスチャも同期する
    impl->textureState->syncPlatform();
}

std::shared_ptr<IRenderingSurface> GLDevice::getSurface() const {
    return impl;
}

Object::QueryResult_e GLDevice::queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const {
    PGD_SUPPORT_QUERY(InterfaceId_OpenGL_Device, GLDevice);
    return Object::queryInterface(interfaceId, resultInterfacePtr);
}


void GLDevice::clearBuffer(const uint32_t clearFlags) {
    GLuint flag = 0;
    if (clearFlags & ClearFlag_Color) {
        flag |= GL_COLOR_BUFFER_BIT;
    }
    if (clearFlags & ClearFlag_Depth) {
        flag |= GL_DEPTH_BUFFER_BIT;
    }
    if (clearFlags & ClearFlag_Stancil) {
        flag |= GL_DEPTH_BUFFER_BIT;
    }

    if (!flag) {
        return;
    }
    glClear(flag);
    assert_gl();
}

IDevice::surface_state GLDevice::getCurrentState() const {
    return impl->surfaceStates.top();
}

void GLDevice::pushSurfaceState(const IDevice::surface_state &state) {
    const surface_state old = getCurrentState();
    setSurfaceState(old, state);
    impl->surfaceStates.push(state);
}

void GLDevice::setSurfaceState(const IDevice::surface_state &state) {
    const surface_state old = getCurrentState();
    setSurfaceState(old, state);
    impl->surfaceStates.top() = state;
}

void GLDevice::setSurfaceState(const IDevice::surface_state &old, const IDevice::surface_state &state) {
    if (old.backbuffer != state.backbuffer) {
        glClearColor(state.backbuffer.rf(), state.backbuffer.gf(), state.backbuffer.bf(), state.backbuffer.af());
        assert_gl();
    }

    if (old.scissor != state.scissor) {
        if (state.scissor.isEmpty()) {
            glDisable(GL_SCISSOR_TEST);
        } else {
            glEnable(GL_SCISSOR_TEST);
            glScissor(state.scissor.left, state.scissor.top, state.scissor.width(), state.scissor.height());
        }
        assert_gl();
    }

    if (old.viewport != state.viewport) {
        glViewport(state.viewport.left, state.viewport.top, state.viewport.width(), state.viewport.height());
        assert_gl();
    }
}

void GLDevice::popSurfaceState() {
    const auto old = getCurrentState();
    impl->surfaceStates.pop();
    assert(impl->surfaceStates.size() > 0);

    setSurfaceState(old, impl->surfaceStates.top());
}

void GLDevice::setRenderState(const render_state &state) {
    const render_state &old = impl->renderState;
    const uint64_t oldFlags = old.flags;
    const uint64_t flags = state.flags;

    // 差分をチェックする
    const uint64_t diffFlags = oldFlags ^state.flags;

    if (diffFlags & render_state::Flag_DepthTest_Enable) {
        // 深度チェックが切り替わった
        if (flags & render_state::Flag_DepthTest_Enable) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        assert_gl();
    }

    // back cullingを切り替える
    if (diffFlags & render_state::Flag_Cull_Back) {
        if (flags & render_state::Flag_Cull_Back) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        } else {
            glDisable(GL_CULL_FACE);
        }
        assert_gl();
    } else if (diffFlags & render_state::Flag_Cull_Front) {
        // frontが切り替わった
        if (flags & render_state::Flag_Cull_Front) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
        } else {
            glDisable(GL_CULL_FACE);
        }
        assert_gl();
    }

    // 表ポリゴンの向きをチェック
    if (diffFlags & render_state::Flag_FrontFace_CW) {
        if (flags & render_state::Flag_FrontFace_CW) {
            // 表ポリゴン判定を反転する
            glFrontFace(GL_CW);
        } else {
            // 表ポリゴン判定をデフォルトにする
            glFrontFace(GL_CCW);
        }
        assert_gl();
    }

    // stencil test
    if (diffFlags & render_state::Flag_StencilTest_Enable) {
        // ステンシルチェックが切り替わった
        if (flags & render_state::Flag_StencilTest_Enable) {
            glEnable(GL_STENCIL_TEST);
        } else {
            glDisable(GL_STENCIL_TEST);
        }
        assert_gl();
    }

    // ブレンディング
    if (old.blendType != state.blendType) {
        static const GLenum sfactor[] = {GL_SRC_ALPHA, GL_SRC_ALPHA,};
        static const GLenum dfactor[] = {GL_ONE_MINUS_SRC_ALPHA, GL_ONE};

        if (state.blendType == render_state::BlendType_None ||
            state.blendType == render_state::BlendType_Unknown) {
            // no blend
            glDisable(GL_BLEND);
        } else {
            glEnable(GL_BLEND);
            glBlendFunc(sfactor[state.blendType], dfactor[state.blendType]);
        }
        assert_gl();
    }

    const uint8_t newLineWidth = std::min<uint8_t>(state.lineWidth, impl->caps->getMaxUniformVectorsFs());
    if (newLineWidth && (newLineWidth != old.lineWidth)) {
        glLineWidth((float) newLineWidth);
        assert_gl();
    }

    // 古いステートを書き換える
    impl->renderState = state;
    impl->renderState.lineWidth = newLineWidth;
}

void GLDevice::unbind(const uint32_t resourceFlags) {
    const auto caps = impl->caps;
    if (resourceFlags & ResourceFlag_Mesh) {
        if (caps->isSupport(IGPUCapacity::GPUExtension_VertexArrayObject)) {
            glBindVertexArray(0);
        } else {
            // for ES 2.0
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    if (resourceFlags & ResourceFlag_FrameBuffer) {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    if (resourceFlags & ResourceFlag_Shader) {
        glUseProgram(0);
    }

    assert_gl();
}

std::shared_ptr<IDisplayTransfer2D> GLDevice::getDisplayTransfer2D() const {
    return impl;
}

std::shared_ptr<GLTextureState> GLDevice::getTextureState() const {
    return impl->textureState;
}

const GLGPUCapacity &GLDevice::getCapacity() const {
    return *impl->caps;
}

void GLDevice::dispose() {
    if (impl) {
        unbind(0xFFFFFFFF);
        glFinish();

        impl.reset();
    }
}
}
}
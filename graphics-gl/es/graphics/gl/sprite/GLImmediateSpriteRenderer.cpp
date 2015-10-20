#include "GLImmediateSpriteRenderer.h"
#include <es/asset/IAsset.hpp>
#include <es/asset/AssetManager.h>
#include "es/graphics/gl/sprite/GLQuadPolygon.h"
#include <es/graphics/gl/resource/GLShaderProgram.h>
#include <es/internal/log/Log.h>
#include "es/graphics/gl/resource/GLTexture.h"

namespace es {
namespace gl {


bool GLImmediateSpriteRenderer::initialize(sp<IDevice> originDevice, std::shared_ptr<AssetManager> assets) {
    assert(originDevice);
    assert(assets);

    GLDevice::query device(originDevice);
    assert(device);

    setDevice(device);

    {
        auto vert = assets->load("gl/shader/SpriteShader.vsh");
        auto frag = assets->load("gl/shader/SpriteShader.fsh");
        this->shader = GLShaderProgram::build(device->getCapacity(), util::toString(vert), util::toString(frag), GLGPUCapacity::GLSLVersion_100);
        assert(shader);
        if (!shader) {
            return false;
        }

        // uniformを設定する
        {
            uniform.poly_data.setLocation(shader->getUniformLocation("poly_data"));
            uniform.poly_uv.setLocation(shader->getUniformLocation("poly_uv"));
            uniform.texture.setLocation(shader->getUniformLocation("tex"));
            uniform.color.setLocation(shader->getUniformLocation("blendColor"));
            uniform.aspect.setLocation(shader->getUniformLocation("aspect"));
            uniform.rotate.setLocation(shader->getUniformLocation("rotate"));
            uniform.fillMode.setLocation(shader->getUniformLocation("fillMode"));
        }
    }

    if (!quad) {
        quad.reset(new GLQuadPolygon());
        // attrを設定する
        quad->updateVertices(originDevice, nullptr, shader->getAttribLocation("vPosition"), shader->getAttribLocation("vTexCoord"));
    }
    device->unbind(GLDevice::ResourceFlag_All);

    return true;
}

void GLImmediateSpriteRenderer::setDevice(GLDevice::query device) {
    this->device = device;
}

void GLImmediateSpriteRenderer::beginRendering(SpriteRenderer *sender) {
    assert(sender && device && shader && quad);

    auto state2d = render_state::createDefault2D();
    device->setRenderState(state2d);
    shader->bind();
    quad->bind();
}

int GLImmediateSpriteRenderer::requestRendering(SpriteRenderer *sender, const ISpriteRenderingCallback::RenderingState *state, const uint numInstances, ISpriteRenderingCallback::RenderingInstance *instanceArray) {
    float oldLineWidth = -1;
    int numRendering = 0;
    if (state->mode == RenderingMode_QuadFill || state->mode == RenderingMode_QuadOutLine || state->mode == RenderingMode_Text) {
        RenderingQuadInstance *quadInstances = (RenderingQuadInstance *) instanceArray;
        for (int i = 0; i < numInstances; ++i) {

            if (quadInstances->texture) {
                // テクスチャにキャストできなければならない
                GLTexture *texture = GLTexture::query::from(quadInstances->texture);
                assert(texture);

                uniform.texture.upload(device.get(), texture);

                float uv_width = quadInstances->srcCoord.right - quadInstances->srcCoord.left;
                float uv_height = quadInstances->srcCoord.bottom - quadInstances->srcCoord.top;

                uniform.poly_uv.upload(quadInstances->srcCoord.left, quadInstances->srcCoord.top,
                                       uv_width, uv_height);

                if (state->mode == RenderingMode_Text) {
                    uniform.fillMode.upload(2);
                } else {
                    uniform.fillMode.upload(0);
                }
            } else {
                uniform.fillMode.upload(1);
            }

            {
                const float sizeX = quadInstances->dstQuad.width();
                const float sizeY = quadInstances->dstQuad.height();
                const float sx = quadInstances->dstQuad.left;
                const float sy = quadInstances->dstQuad.top;
                const float translateX = sizeX / 2.0f + sx;
                const float translateY = -sizeY / 2.0f + sy;
                // ポリゴン位置の確定
                uniform.poly_data.upload(translateX, translateY, sizeX, sizeY);

            }
            // ブレンド色を設定する
            uniform.color.upload(quadInstances->color);
            // ポリゴン回転を設定する
            uniform.rotate.upload(quadInstances->rotateRadian);
            // アスペクト比を転送する
            uniform.aspect.upload(state->surfaceAspect);

            // ラインの太さを決める
            if (state->mode == RenderingMode_QuadOutLine) {
                if (oldLineWidth != quadInstances->lineWidth) {
                    assert(quadInstances->lineWidth > 0);
                    oldLineWidth = std::min<float>(quadInstances->lineWidth, (float) device->getCapacity().getMaxLineWidth());
                    glLineWidth(oldLineWidth);
                    assert_gl();
                }
                quad->setPrimitiveType(GL_LINE_LOOP);
            } else {
                quad->setPrimitiveType(GL_TRIANGLE_FAN);
            }

            quad->rendering();

            ++quadInstances;
        }
    }
    return numRendering;
}

void GLImmediateSpriteRenderer::endRendering(SpriteRenderer *sender) {
    assert(sender && device && shader && quad);
    device->unbind(GLDevice::ResourceFlag_All);
}

}
}
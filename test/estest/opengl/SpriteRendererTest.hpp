#pragma once

#include <es/graphics/gl/sprite/GLImmediateSpriteRenderer.h>
#include "estest/protoground-test.hpp"
#include "es/graphics/2d/SpriteRenderer.h"
#include "es/graphics/gl/resource/GLShaderProgram.h"
#include "es/graphics/gl/sprite/GLImmediateSpriteRenderer.h"
#include "es/math/Math.hpp"

namespace es {
namespace gl {
namespace test {

/**
 * 正常にデバイス座標に対して射影出来ることを確認する
 */
TEST(SpriteRendererTest, DeviceProjection) {
    gl::internal::UnitTestEngine engine;
    
    // 結果を計算しやすいViewportに設定
    auto transfer = engine.device->getDisplayTransfer2D();

    {
        IDevice::surface_state state;
        state.viewport.setXYWH(0, 0, 200, 100);
        engine.device->setSurfaceState(state);

        vec2 leftTop = transfer->getSpritePositionToDevice(0, 0);
        vec2 rightBottom = transfer->getSpritePositionToDevice(200, 100);
        vec2 center = transfer->getSpritePositionToDevice(100, 50);

        ASSERT_TRUE(es::equals(leftTop.x, -1.0f));
        ASSERT_TRUE(es::equals(leftTop.y, 1.0f));

        ASSERT_TRUE(es::equals(rightBottom.x, 1.0f));
        ASSERT_TRUE(es::equals(rightBottom.y, -1.0f));

        ASSERT_TRUE(es::equals(center.x, 0.0f));
        ASSERT_TRUE(es::equals(center.y, 0.0f));
    }

    class TestingSpriteRenderer : public gl::GLImmediateSpriteRenderer {
    public:
        int callbackNum = 0;

        virtual int requestRendering(SpriteRenderer *sender, const RenderingState *state, const uint numInstances, RenderingInstance *instanceArray) {
            int result = gl::GLImmediateSpriteRenderer::requestRendering(sender, state, numInstances, instanceArray);

            ++callbackNum;
            assert(es::equals(state->surfaceAspect, 200.0f / 100.0f));

            return result;
        }

    };

    sp<SpriteRenderer> spriteRenderer(new SpriteRenderer());
    sp<TestingSpriteRenderer> callback(new TestingSpriteRenderer());
    callback->initialize(engine.device, IProcessContext::getInstance()->getAssetManager());

    spriteRenderer->setDisplay(engine.device->getDisplayTransfer2D());
    spriteRenderer->setCallback(callback);

    spriteRenderer->begin();
    spriteRenderer->renderingRect(0, 0, 200, 100, Color::white());
    spriteRenderer->renderingRect(0, 0, 0, 100, Color::white());
    spriteRenderer->renderingRect(0, 0, 200, 0, Color::white());
    spriteRenderer->end();

    // 無駄なコールバックが呼ばれていないことを確認する
    ASSERT_EQ(callback->callbackNum, 1);
}
    
}
}
}

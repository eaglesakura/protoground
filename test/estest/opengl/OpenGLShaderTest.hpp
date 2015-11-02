#pragma once

#include "estest/protoground-test.hpp"

#include "es/graphics/gl/engine/OpenGLEngine.h"
#include "es/graphics/gl/engine/GLGPUCapacity.h"
#include <es/graphics/gl/resource/GLShaderProgram.h>
#include <es/graphics/gl/engine/GLDevice.h>
#include "es/graphics/gl/debug/GLPrimitiveRenderer.h"

namespace es {
namespace test {

/**
 * スプライト用のデフォルトシェーダーが正常にビルドできることを確認する
 */
TEST(OpenGLShaderTest, SpriteShaderCompile) {
    gl::internal::UnitTestEngine engine;

    auto vsh = IProcessContext::getInstance()->getAssetManager()->load("gl/shader/SpriteShader.vsh");
    auto fsh = IProcessContext::getInstance()->getAssetManager()->load("gl/shader/SpriteShader.fsh");
    ASSERT_TRUE((bool) vsh);
    ASSERT_TRUE((bool) fsh);

    auto shader = gl::GLShaderProgram::build(engine.glDevice->getCapacity(), util::toString(vsh), util::toString(fsh), gl::GLGPUCapacity::GLSLVersion_100);

    ASSERT_TRUE((bool) shader);

    eslog("GLSL Version(%d)", shader->getGLSLVersion());
}

TEST(OpenGLShaderTest, DebugShaderCompile) {
    gl::internal::UnitTestEngine engine;

    auto vsh = IProcessContext::getInstance()->getAssetManager()->load("gl/debug/wire.vsh");
    auto fsh = IProcessContext::getInstance()->getAssetManager()->load("gl/debug/wire.fsh");
    ASSERT_TRUE((bool) vsh);
    ASSERT_TRUE((bool) fsh);


    auto shader = gl::GLShaderProgram::build(engine.glDevice->getCapacity(), util::toString(vsh), util::toString(fsh), gl::GLGPUCapacity::GLSLVersion_100);

    ASSERT_TRUE((bool) shader);

    eslog("GLSL Version(%d)", shader->getGLSLVersion());
}


}
}
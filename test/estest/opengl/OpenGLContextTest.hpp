#pragma once

#include "estest/protoground-test.hpp"
#include "es/graphics/gl/engine/OpenGLEngine.h"
#include "es/graphics/gl/engine/GLGPUCapacity.h"

namespace es {
namespace test {

TEST(GLEngineTest, InitContext) {
    gl::internal::UnitTestEngine engine;

    auto device = engine.engine->getCurrentDevice();
    gl::GLGPUCapacity::query cap(engine.engine->getCapacity());

    ASSERT_TRUE((bool) device);
    ASSERT_TRUE((bool) device);

    ASSERT_NE(cap->getGPUFamily(), IGPUCapacity::GPUFamily_Unknown);
    ASSERT_NE(cap->getGLVersion(), gl::GLGPUCapacity::GLVersion_Unknown);
}

}
}
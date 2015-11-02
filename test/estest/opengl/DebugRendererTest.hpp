#pragma once

#include "estest/protoground-test.hpp"
#include "es/graphics/gl/debug/GLPrimitiveRenderer.h"


namespace es {
namespace gl {
namespace test {

TEST(DebugRendererTest, InitializeTest) {
    gl::internal::UnitTestEngine engine;

    sp<debug::GLPrimitiveRenderer> render(new debug::GLPrimitiveRenderer());

    VertexAttribute attribute[] = {
            VertexAttribute::POSITION_float3,
            VertexAttribute::COLOR_i4,
            VertexAttribute::END
    };

    render->initialize(engine.device, VertexAttribute::makeComplex(attribute), IProcessContext::getInstance()->getAssetManager());
}

}
}
}
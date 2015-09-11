#pragma once

#include "estest/protoground-test.hpp"
#include "es/graphics/3d/camera/SceneLens.h"

namespace es {
namespace test {

TEST(SceneLensTest, FocalLength24mm) {
    const float SAMPLE_LENGTH = 24;

    SceneLens lens;
    lens.setFocalLength(SAMPLE_LENGTH);
    ASSERT_EQ(lens.getFocalLength(), SAMPLE_LENGTH);

    const auto fovY = lens.getFovY();
    ASSERT_EQ((int) (fovY), (int) (53.1301f));
}

TEST(SceneLensTest, FocalLength35mm) {
    const float SAMPLE_LENGTH = 35;

    SceneLens lens;
    lens.setFocalLength(SAMPLE_LENGTH);
    ASSERT_EQ(lens.getFocalLength(), SAMPLE_LENGTH);

    const auto fovY = lens.getFovY();
    ASSERT_EQ((int) (fovY), (int) (37.8493f));
}

TEST(SceneLensTest, FocalLength50mm) {
    const float SAMPLE_LENGTH = 50;

    SceneLens lens;
    lens.setFocalLength(SAMPLE_LENGTH);
    ASSERT_EQ(lens.getFocalLength(), SAMPLE_LENGTH);

    const auto fovY = lens.getFovY();
    ASSERT_EQ((int) (fovY), (int) (26.9914f));
}


}
}
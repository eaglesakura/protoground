#include "SceneLens.h"
#include "es/math/protoground-glm.hpp"
#include <cmath>

namespace es {

SceneLens::SceneLens() {
    setFocalLength(35); /* デフォルトは35mmレンズ */
}

SceneLens::SceneLens(const float focalLen) {
    setFocalLength(focalLen);
}

/**
 * 計算式参考 : http://keisan.casio.jp/exec/system/1378259716
 */
void SceneLens::setFocalLength(const float length) {
    this->focalLength = length;
    this->fovY = (float) glm::degrees((2.0 * atan(24.0 / (2.0 * length))));
}

}
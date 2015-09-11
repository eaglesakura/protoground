#pragma once

#include "es/protoground.hpp"

#include "es/math/Rect.hpp"

namespace es {

/**
 * 2D座標系の変換を行わせる
 */
class IDisplayTransfer2D {
public:
    /**
     * スプライト座標を正規化デバイス座標に変換する
     */
    virtual vec2 getSpritePositionToDevice(const int x2D, const int y2D) const = 0;

    /**
     * サーフェイスのアスペクト比を取得する
     */
    virtual float getSpriteSurfaceAspect() const = 0;

    virtual ~IDisplayTransfer2D() = default;
};

}
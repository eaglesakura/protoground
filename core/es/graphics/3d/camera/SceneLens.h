#pragma once

#include "es/protoground.hpp"

namespace es {

/**
 * シーン撮影用のレンズ情報を構築する。
 *
 * 入力は35mmフィルム換算の焦点距離で行う。
 * デフォルトは35mm換算50mmレンズ。
 */
class SceneLens : public Object {
public:
    SceneLens();

    SceneLens(const float focalLen);

    virtual ~SceneLens() = default;

    /**
     * 焦点距離を指定する。
     *
     * 必ず35mmフィルム換算で入力すること。
     */
    void setFocalLength(const float length);


    /**
     * 焦点距離を取得する
     */
    float getFocalLength() const {
        return focalLength;
    }

    /**
     * Y方向画角を取得する
     */
    float getFovY() const {
        return fovY;
    }

private:
    /**
     * 焦点距離
     */
    float focalLength = 0;

    /**
     * 計算済みのdegree画角
     */
    float fovY = 0;

#if 0
    /**
     * F値
     */
    float fNumber = 0;
#endif
};

}


